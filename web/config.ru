require 'sinatra/base'
require './web/helpers/application_helper.rb'
require './web/controllers/application_controller'

require './web/controllers/web_dash_controller'

map('/') { run WeatherDashController }
