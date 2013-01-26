require 'bundler/setup'
require 'erb'
require 'sinatra'
require 'haml'
require 'rnexus'

get '/' do
  @plotter = Rnexus::Plotter.new('config')
  temp_data = @plotter.get_last_24h(:T5).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
  temp_values = temp_data.inspect
  press_data =  @plotter.get_last_24h(:PRESS).map {|d| d[1] }
  press_values = press_data.inspect
  humaditity_data =  @plotter.get_last_24h(:H5).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
  humaditity_values = humaditity_data.inspect
	rain_data = @plotter.get_last_24h(:RC)
	rain_value = rain_data.first[1] - rain_data.last[1]
	puts rain_value
  start_unixtime = temp_data.first[0]
  chart_title = "Last 24h"
  erb = ERB.new(File.read("web/temperature_line.js.erb"))
  @temperature_line_js = erb.result(binding)
  erb = ERB.new(File.read("web/rain_gauge.js.erb"))
  @rain_gauge_js = erb.result(binding)  
  erb = ERB.new(File.read("web/press_column.js.erb"))
  @press_column_js = erb.result(binding)  
  haml :index
end
