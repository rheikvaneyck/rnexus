require 'bundler/setup'
require 'erb'
require 'sinatra'
require 'haml'
require 'rnexus'

get '/' do
  @db = Rnexus::DBManager.new('db')
  temp_data = Rnexus::DBManager::Measurement.all.map(&:T5)
  chart_title = "Outside Temperature"
  erb = ERB.new(File.read("web/temperature_line.js.erb"))
  @temperature_line_js = erb.result(binding)
  haml :index
end
