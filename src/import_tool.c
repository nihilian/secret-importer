/*
 *  Copyright (C) 2020 Benjamin Allred <nihilian@live.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "password_entry.h"
#include "secret_schema_web.h"

SecretCollection *select_secret_collection(SecretService *service)
{
  if (!service) {
    fprintf(stderr, "line %d: pointer to type SecretService is NULL\n",
            __LINE__);
    return NULL;
  }

  GList *collection_list = secret_service_get_collections(service);
  if (!collection_list) {
    fprintf(stderr, "line %d: secret_service_get_collections() returned NULL\n",
            __LINE__);
    return NULL;
  }

  printf("\nWhich Secret Collection (i.e. keyring) should be used to store Web"
         " passwords?\n\nSelection\t\tSecret Collection Name\n"
         "--------------------------------------------------------\n");
  GList *collection_cur = collection_list;
  int collection_cur_index = 0;
  for (; collection_cur; collection_cur = collection_cur->next) {
    const gchar *label =
      secret_collection_get_label((SecretCollection *)collection_cur->data);
    printf("[%d]\t\t\t%s\n", collection_cur_index, label);
    g_free((gpointer)label);
    ++collection_cur_index;
  }
  printf("\nEnter selection[0-%d], or anything else to cancel: ",
         collection_cur_index - 1);

  int sel;
  if (!(scanf("%d", &sel) == 1 && sel >= 0 && sel < collection_cur_index)) {
    return NULL;
  }

  return (SecretCollection *)g_list_nth_data(collection_list, sel);
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "expected: filepath of passwords CSV\n");
    return EXIT_FAILURE;
  }

  FILE *fd;
  if (!(fd = fopen(argv[1], "r"))) {
    fprintf(stderr, "line %d: fopen() filepath=\"%s\":\n%s\n", __LINE__,
            argv[1], strerror(errno));
    return EXIT_FAILURE;
  }

  char name[256], url[256], username[256], password[256];
  int n1, n2, n3, n4, ret;
  if ((ret = fscanf(fd, "%255[^,]%n,%255[^,]%n,%255[^,]%n,%255[^\n]%n\n",
                    name, &n1, url, &n2, username, &n3, password, &n4)) != 4) {
    fprintf(stderr, "line %d: invalid CSV format\n", __LINE__);
    fclose(fd);
    return EXIT_FAILURE;
  }

  PasswordEntry *entry_list = alloc_password_entry(name, n1,
                                                   url, n2 - n1 - 1,
                                                   username, n3 - n2 - 1,
                                                   password, n4 - n3 - 1);
  PasswordEntry *entry_cur = entry_list;
  int entry_count = 0;
  while ((ret = fscanf(fd, "%255[^,]%n,%255[^,]%n,%255[^,]%n,%255[^\n]%n\n",
                       name, &n1,
                       url, &n2,
                       username, &n3,
                       password, &n4)) > 0) {
    int error_line = 0;
    switch (ret) {
      case 4:
        n4 -= n3 + 1;
        n3 -= n2 + 1;
        n2 -= n1 + 1;
        break;
      case 3:
        ret = fscanf(fd, "\n");
        error_line = ret != 0 ? __LINE__ : 0;
        password[0] = '\0';
        n4 = 0;
        n3 -= n2 + 1;
        n2 -= n1 + 1;
        break;
      case 2:
        ret = fscanf(fd, ",%255[^\n]%n\n", password, &n4);
        error_line = ret != 1 ? __LINE__ : 0;
        username[0] = '\0';
        n4 -= 1;
        n3 = 0;
        n2 -= n1 + 1;
        break;
      default:
        error_line = __LINE__;
    }

    if (error_line) {
      fprintf(stderr, "line %d: fscanf() returned %d\n", error_line, ret);
      fclose(fd);
      free_password_entry_list(entry_list);
      return EXIT_FAILURE;
    }

    entry_cur->next = alloc_password_entry(name, n1,
                                           url, n2,
                                           username, n3,
                                           password, n4);
    entry_cur = entry_cur->next;
    ++entry_count;
  }

  int errnum = errno;
  int errflag = ferror(fd) || !feof(fd);
  fclose(fd);

  if (errflag) {
    fprintf(stderr, "line %d: error parsing \"%s\":\n%s\n", __LINE__,
            argv[1], strerror(errnum));
    free_password_entry_list(entry_list);
    return EXIT_FAILURE;
  }

  GError *error = NULL;
  SecretService *service =
    secret_service_get_sync(SECRET_SERVICE_LOAD_COLLECTIONS, NULL, &error);
  if (error) {
    fprintf(stderr, "line %d: secret_service_get_sync():\n%s\n", __LINE__,
            error->message);
    g_error_free(error);
    free_password_entry_list(entry_list);
    return EXIT_FAILURE;
  }

  SecretCollection *collection = select_secret_collection(service);
  if (!collection) {
    g_object_unref(service);
    free_password_entry_list(entry_list);
    return EXIT_FAILURE;
  }

  const gchar *collection_dbus_path =
    g_dbus_proxy_get_object_path((GDBusProxy *)collection);

  int import_count = 0;
  int entry_cur_index = 1;
  for (entry_cur = entry_list->next; entry_cur; entry_cur = entry_cur->next) {
    error = NULL;
    if (secret_password_store_sync(SECRET_SCHEMA_WEB,
                                   collection_dbus_path,
                                   entry_cur->name,
                                   entry_cur->password,
                                   NULL,
                                   &error,
                                   "url", entry_cur->url,
                                   "username", entry_cur->username,
                                   NULL)) {
      ++import_count;
    } else if (error) {
      fprintf(stderr, "failed to store secret entry %d, %s:\n%s\n",
              entry_cur_index, entry_cur->name, error->message);
      g_error_free(error);
    } else {
      fprintf(stderr, "failed to store secret entry %d, %s:\nunknown error\n",
              entry_cur_index, entry_cur->name);
    }
    ++entry_cur_index;
  }

  g_object_unref(service);
  free_password_entry_list(entry_list);

  printf("Successfully imported %d/%d entries\n", import_count, entry_count);

  return entry_count - import_count;
}
