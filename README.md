rnexus
======

Dashboard for Weather Station FTA Nexus 

Currently under development

Prepare the app with the tools under 'script'.
  1. load_weather_data.sh unter 'load_weather_data' to load data from weather station
  2. create_migration.rb under 'create_scheme' to generate the activerecord migration
  2b. create_state_migration.rb under 'create_scheme' to generate the activerecord migration
  3. rake db:migrate to create the databas
  4. rake db:load_test_data to load the DB with test weather data
