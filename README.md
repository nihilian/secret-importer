# Secret Service Web Password Import Tool
Import passwords from your web browser's password storage to your desktop's
keyring/wallet service. This tool should work with any keyring/wallet that
implements the Secret Service API (e.g. GNOME Keyring, KWallet, etc.).

For information about the Secret Service API, see:
<https://specifications.freedesktop.org/secret-service/>

Currently, Chromium/Chrome is the only supported web browser.

## Installing dependencies (Ubuntu/Debian-based)
```
sudo apt install autoconf automake build-essential libsecret-1-dev pkg-config
```

## Building from source
```
./autogen.sh
./configure
make
```

Optional:
```
make install
```

## Importing passwords

### Chromium/Chrome
From `chrome://settings/passwords` there is an option to export passwords.

**CAUTION: passwords will be saved as a plain-text CSV file. This is currently
the only way to import them using this tool.**

Ensure that the exported CSV file begins with this line:
```
name,url,username,password
```

Run:
```
secret-importer /path/to/csv
```
You will be prompted to select the collection (aka keyring in some
implementations) where your passwords will be stored. Any runtime errors will be
printed to `stderr`.

**Remember to delete the CSV file afterwards to avoid having all your secrets
out in the open.**
