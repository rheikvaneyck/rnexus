require 'bundler/setup'
require 'erb'
require 'sinatra'
require 'haml'
require 'rnexus'

get '/' do
  battery_states = ["/images/battery-0-r.png","/images/battery-100-r.png"]
  weather_forecast = [
    "/images/snow-shower.png",
    "/images/snow.png",
    "/images/rain.png",
    "/images/drizzle.png",
    "/images/clouds.png",
    "/images/cloudy.png",
    "/images/sunny.png" ]
=begin
weather_forcast:
  0: heavy snow
  1: little snow
  2: heavy rain
  3: little rain
  4: cloudy
  5: some clouds
  6: sunny
=end
  @plotter = Rnexus::Plotter.new('config')
  @fc = weather_forecast[@plotter.get_last_measurement().FC]
  @status = Rnexus::Status.new('config')
  @state = @status.get_last_state
  @batteries = [battery_states[@state.BAT1],battery_states[@state.BAT5],battery_states[@state.BATR],battery_states[@state.BATW]]
  
  temp_data = @plotter.get_last_24h(:T5).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
  temp_values = temp_data.inspect
  
  press_data =  @plotter.get_last_24h(:PRESS).map {|d| d[1] }
  press_values = press_data.inspect
  
  humaditity_data =  @plotter.get_last_24h(:H5).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
  humaditity_values = humaditity_data.inspect
	
  rain_data = @plotter.get_last_24h(:RC)
	rain_value = rain_data.last[1] - rain_data.first[1]
  
  wind_direction = @plotter.get_last_24h(:WD).map {|d| d[1] }
  wind_speed = @plotter.get_last_24h(:WS).map {|d| d[1] }
  # FIXME CALC wind array (prob with array.zip()? 
  wind = wind_direction.zip(wind_speed)
  wind_groups = wind.group_by {|w| w[0] }
  wind_sums = Array.new(16,0) 
  wind_groups.each {|k,v| sum = 0.0; v.map {|i| sum += i[1].to_f }; wind_sums[k.to_i - 1] = (sum / v.length) }
  wind_values = wind_sums.inspect

  start_unixtime = temp_data.first[0]
  chart_title = "Last 24h"
  erb = ERB.new(File.read("web/temperature_line.js.erb"))
  @temperature_line_js = erb.result(binding)
  erb = ERB.new(File.read("web/rain_gauge.js.erb"))
  @rain_gauge_js = erb.result(binding)  
  erb = ERB.new(File.read("web/press_column.js.erb"))
  @press_column_js = erb.result(binding)  
  erb = ERB.new(File.read("web/wind_polar.js.erb"))
  @wind_polar_js = erb.result(binding)  

  haml :index
end
