require 'bundler/setup'
require 'erb'
require 'haml'
require 'rnexus'

class WeatherDashController < ApplicationController

  get '/' do
    # FIXME: source image constants to config file
    battery_states = ["/images/battery-0-r.png","/images/battery-100-r.png"]
    weather_forecast = [
      "/images/snow.svg",
      "/images/snow-shower.svg",
      "/images/rain.svg",
      "/images/drizzle.svg",
      "/images/clouds.svg",
      "/images/cloudy.svg",
      "/images/sunny.svg" ]
  # weather_forcast:
  #   0: heavy snow
  #   1: little snow
  #   2: heavy rain
  #   3: little rain
  #   4: cloudy
  #   5: some clouds
  #   6: sunny
    @plotter = Rnexus::Plotter.new(File.join(File.dirname(__FILE__), '..', 'config'))
    @fc = weather_forecast[@plotter.get_last_measurement().FC]
    @status = Rnexus::Status.new(File.join(File.dirname(__FILE__), '..', 'config'))
    @state = @status.get_last_state
    @batteries = [battery_states[@state.BAT1],battery_states[@state.BAT3],battery_states[@state.BATR],battery_states[@state.BATW]]

    # probing for acitve Sensors
    # because there is one device for tempereture AND humidity, we will only probe temperatur sensors
    # Initialize the activity of all sensors to false
    # so we have a complete list of all sensors activity
    @sensors = [[:T1, :H1, false], [:T2, :H2, false], [:T3, :H3, false], [:T4, :H4, false], [:T5, :H5, false]]
    m = @plotter.get_last_measurement
    @sensors.each do |sensor|
    	sensor[2] = true unless m.send(sensor[1]) == 0
    end
    
    # Find the first acitve sensor
    sensor = @sensors.detect {|s| s[2] == true }
    if sensor.nil?
      ACTIVE_TEMP_SENSOR = :T0
      ACTIVE_HUMI_SENSOR = :H0
    else
      ACTIVE_TEMP_SENSOR = sensor[0]
      ACTIVE_HUMI_SENSOR = sensor[1]
    end

    temp_data = @plotter.get_last_24h(ACTIVE_TEMP_SENSOR).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }

    press_data = @plotter.get_last_24h(:PRESS).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }
    press_data_min = press_data.min {|a,b| a[1] <=> b[1] }[1]

    humidity_data = @plotter.get_last_24h(ACTIVE_HUMI_SENSOR).map {|d| [DateTime.parse(d[0]).to_time.to_i * 1000, d[1]] }

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

    week_extrems_data = @plotter.get_days_min_max(7, ACTIVE_TEMP_SENSOR)
    week_dates = week_extrems_data.map{|d| DateTime.parse(d[0]).to_time.to_i * 1000}
    week_dates_values = week_dates.inspect
    weeks_min_values = week_extrems_data.map{|d| d[1]}.inspect
    weeks_max_values = week_extrems_data.map{|d| d[2]}.inspect

    start_unixtime = temp_data.first[0]
    week_start_unixtime = week_dates.first
    chart_title = "Last 24h"
    erb = ERB.new(File.read("views/temperature_line.js.erb"))
    @temperature_line_js = erb.result(binding)
    erb = ERB.new(File.read("views/rain_gauge.js.erb"))
    @rain_gauge_js = erb.result(binding)
    erb = ERB.new(File.read("views/temp_extrems.js.erb"))
    @temp_extrems_js = erb.result(binding)
    erb = ERB.new(File.read("views/wind_polar.js.erb"))
    @wind_polar_js = erb.result(binding)

    haml :index
  end
end
