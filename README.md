rnexus
======

Web based dashboard for the weather station FTA Nexus 

**Currently under development**

rnexus is a small web based dashboard using Ruby's sinatra library as application middleware. 

Dependencies
------------

The application presents the data read from the FTA Nexus weather station. The tool used for accessing the data via the USB port is **t923tool** from Sven John ( http://te923.fukz.org/ ). Though this tool has some restrictions with the FTA Nexus (reads only max ~770 entries from memory and doesn't read the solar sensor), the tool is still usefull. 

Download te923tool from its website and compile the source: (You should have the libusb-dev installed).

```
wget http://te923.fukz.org/downloads/te923tool-0.5.tgz
tar xfz  te923tool-0.5.tgz
cd te923tool-0.5
make
sudo cp te923con /usr/bin
```

The Ruby gems used are listed in the Gemfile. You can use the bundler gem to install them automaticly. If you have the **bundler** installed the next step after downloading the source is a call of `bundle install` in the source directory.

Download the application
------------------------

Let's Make a local copy of the source tree of the application with git:

```
git clone git://github.com/rheikvaneyck/rnexus.git
```

Prepare the application
----------------------

The application reads the data from a sqlite database in the db directory. So let's start with reading the stored weather data from the memory to have some data to play with. The next command assume that we are in the source directory.

###Download the data from the weather station

```
te923con -d > data/wetter.data
```

###Create the database

Now we have to import the data in the database. But first we need one. You create that database and the necessary tables with ruby's **rake** tool:

```
rake db:migrate
```

All available rake tasks can be shown with the `rake -T` command.

###Import the data manually

Then you can import the data into the database with a rake task:
```
rake db:load_data
```

###Import the data periodicly

You could use a cron-job to import the data periodicly (e.g. once a hour). If you prefer so, create a file *crontab_pi*  in the directory */etc/cron.hourly* with the following content:

```
17 * * * * /var/local/load-weather-data > /dev/null 2>&1
30 * * * * /var/local/load-data > /dev/null 2>&1
``` 

Make this file executable with `sudo chmod a+x /etc/crontab_pi` and copy the scripts `load-weather-data` from the directory `script/cron/cron.hourly/` and `load-data` from `/script/cron/` to `/var/local/` and make them executable, too. 

This script writes the weather data to `/var/log/weather.data`.

The second script `load-data` copies that data file to the application directory `data/` and loads that file into the database. To do that the application path (`APPDIR=/home/pi/source/rnexus`) in the script has to be edited. 

###Run the application

The application can be started with `rake web:run`. The web application listens on http://localhost:4567/
