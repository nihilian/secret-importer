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

#include "secret_schema_web.h"

const SecretSchema *web_secret_get_schema(void)
{
  static const SecretSchema schema = {
    "ext.nihilian.SecretSchema.Web", SECRET_SCHEMA_NONE,
    {
      { "url",      SECRET_SCHEMA_ATTRIBUTE_STRING },
      { "username", SECRET_SCHEMA_ATTRIBUTE_STRING },
      { NULL, 0 }
    }
  };

  return &schema;
}
