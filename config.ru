require 'sinatra/base'
require File.expand_path '../helpers/application_helper.rb', __FILE__
require File.expand_path '../controllers/application_controller', __FILE__

require File.expand_path '../controllers/web_dash_controller', __FILE__

run WeatherDashController
