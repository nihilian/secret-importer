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

#include <stdlib.h>
#include <string.h>

#include "password_entry.h"

PasswordEntry *alloc_password_entry(const char *name, int name_len,
                                    const char *url, int url_len,
                                    const char *username, int username_len,
                                    const char *password, int password_len)
{
  PasswordEntry *password_entry = malloc(sizeof(PasswordEntry));

  password_entry->name = malloc(name_len + 1);
  strncpy(password_entry->name, name, name_len);
  password_entry->name[name_len] = '\0';

  password_entry->url = malloc(url_len + 1);
  strncpy(password_entry->url, url, url_len);
  password_entry->url[url_len] = '\0';

  password_entry->username = malloc(username_len + 1);
  strncpy(password_entry->username, username, username_len);
  password_entry->username[username_len] = '\0';

  password_entry->password = malloc(password_len + 1);
  strncpy(password_entry->password, password, password_len);
  password_entry->password[password_len] = '\0';

  password_entry->next = NULL;

  return password_entry;
}

void free_password_entry_list(PasswordEntry *entry_list)
{
  if (!entry_list) return;

  free_password_entry_list(entry_list->next);

  free(entry_list->name);
  free(entry_list->url);
  free(entry_list->username);
  free(entry_list->password);
  free(entry_list);
}
