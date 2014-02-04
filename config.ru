require 'sinatra/base'
require File.expand_path '../web/helpers/application_helper.rb', __FILE__
require File.expand_path '../web/controllers/application_controller', __FILE__

require File.expand_path '../web/controllers/web_dash_controller', __FILE__

run WeatherDashController