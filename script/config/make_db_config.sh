#!/bin/sh

if [ $# -lt 1 ]
then
        echo "Usage : $0 development|production"
        echo "No database connection is configured"
        exit
fi

case "$1" in

"production")  echo "create postgres config"
databaseconfig=$(cat <<'EOCONFIG'
adapter: postgresql
host: localhost
username: user
database: app-dev
log_dir: log
EOCONFIG
)
    ;;
*) echo "create sqlite config"
databaseconfig=$(cat <<'EOCONFIG'
adapter: sqlite3d
atabase:  db/weather_db.sqlite
log_dir: log
EOCONFIG
)
   ;;
esac

echo "$databaseconfig" > ../../config/database.yml
[ -r "../../config/database.yml" ] && echo "config for db written and readable"