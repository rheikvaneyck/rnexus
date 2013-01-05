require 'bundler/setup'
require 'erb'
require 'sinatra'
require 'haml'
require 'rnexus'

get '/' do
  @plotter = Rnexus::Plotter.new('config')
  temp_data = @plotter.get_last_24h(:T5).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
  temp_values = temp_data.inspect
  press_data =  @plotter.get_last_24h(:PRESS).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
  press_values = press_data.inspect
  start_unixtime = temp_data.first[0]
  chart_title = "Last 24h"
  erb = ERB.new(File.read("web/temperature_line.js.erb"))
  @temperature_line_js = erb.result(binding)
  haml :index
end
