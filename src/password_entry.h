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

#ifndef PASSWORD_ENTRY_H
#define PASSWORD_ENTRY_H

typedef struct PasswordEntry {
  char *name;
  char *url;
  char *username;
  char *password;
  struct PasswordEntry *next;
} PasswordEntry;

PasswordEntry *alloc_password_entry(const char *name, int name_len,
                                    const char *url, int url_len,
                                    const char *username, int username_len,
                                    const char *password, int password_len);

void free_password_entry_list(PasswordEntry *entry_list);

#endif
