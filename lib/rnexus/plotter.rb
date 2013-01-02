# This lib containts a module to read the database and generate
# images from that data.

module Rnexus
  class Plotter
    def initialize(db_config)
      @db = Rnexus::DBManager.new(db_config)
    end
    def get_chart_data(type = :DT)
      data = Rnexus::DBManager::Measurement.all.map(&type)
      return data
    end
    def get_last_measurement()
      # returns Measurement Object
      measurement = Rnexus::DBManager::Measurement.order("DT DESC").first
      return measurement
    end
    
    def get_last_24h(type = :T0)
      data = get_one_day(dt_str(get_last_measurement.DT - 86400), type)
      return data
    end
    
    def get_last_day(type = :T0)
      # returns array
      data = get_one_day(df_str_s(get_last_measurement.DT), type)
      return data
    end
    
    def get_one_day(day_string, type = :T0)
      # returns array
      start_timestamp = unix_time(day_string)
      end_timestamp = start_timestamp + 86400 # == 24h - 1
      data = Rnexus::DBManager::Measurement.where(:DT => start_timestamp..end_timestamp).map {|m| [dt_str(m.DT),m.send(type.to_sym)]}
      return data
    end
    
    def get_days_min_max(type = :T0)
      # returns array
      data = Rnexus::DBManager::Measurement.all.map {|m| [m.DT, m.send(type.to_sym)]}
      days = {}
      data.each do |d|
        if days[df_str_s(d[0])].nil?
          days[df_str_s(d[0])] = Array.new([d[1]])
        else
          days[df_str_s(d[0])] << d[1]
        end
      end
      dmm = []
      days.each do |key, arr|
        dmm << [key, arr.min, arr.max]
      end
      return dmm
    end
    
    private
    def dt_str(sec)
      Time.at(sec).utc.strftime('%Y-%m-%d %H:%M:%S')
    end
    def df_str_s(sec)
      Time.at(sec).utc.strftime('%Y-%m-%d')
    end
    def unix_time(date_str)
      DateTime.parse(date_str).to_time.to_i
    end
  end
end