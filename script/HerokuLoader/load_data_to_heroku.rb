require 'csv'
require 'active_record'
require 'uri'
require 'yaml'

class HerokuLoader
  def initialize(database_url)
    # FIXME: File paths hard coded
    file_paths = YAML.load_file(File.expand_path("../config.yml", __FILE__))
    @scheme_descr_file = File.expand_path(file_paths["scheme_descr_file"], __FILE__)
    @states_descr_file = File.expand_path(file_paths["states_descr_file"], __FILE__)
    @weather_data_file = File.expand_path(file_paths["weather_data_file"], __FILE__)
    @weather_status_file = File.expand_path(file_paths["weather_status_file"], __FILE__)
    
    db = URI.parse(database_url)
    db_config = {
      :adapter  => db.scheme == 'postgres' ? 'postgresql' : db.scheme,
      :host     => db.host,
      :port     => db.port,
      :username => db.user,
      :password => db.password,
      :database => db.path[1..-1],
      :encoding => 'utf8',
    }

    ActiveRecord::Base.establish_connection(db_config)

  end

  class Measurement < ActiveRecord::Base
    validates_uniqueness_of :DT # FIXME: make this more generell
  end

  class State < ActiveRecord::Base
  end

  def load_data_to_db
    # FIXME: Error handling for files and paths
    scheme_description = YAML.load_file(@scheme_descr_file)
    data = CSV.read(@weather_data_file, { col_sep: ':' })
    
    data.each do |d|
      i = 0
      scheme_description.each do |key, value|
        scheme_description[key] = d[i] unless d[i] == "i"
        i += 1
      end
      
      Measurement.create scheme_description
    end

    scheme_description = YAML.load_file(@states_descr_file)
    data = CSV.read(@weather_status_file, { col_sep: ':' })
    
    data.each do |d|
      i = 0
      scheme_description.each do |key, value|
        scheme_description[key] = d[i] unless d[i] == "i"
        i += 1
      end
      
      State.create scheme_description
    end
  end

  def delete_old_data(cnt_days = 7)
    last_timestamp = Measurement.order("\"DT\" DESC").first.DT
    cut_timestamp = last_timestamp - (86400 * cnt_days)
    Measurement.delete_all(["\"DT\" < ?", cut_timestamp])
  end
end

h = HerokuLoader.new("postgres://htvfnuodrmqvwx:ghpvgl-aEJaXqHAJXAUO8SDuji@ec2-54-225-69-193.compute-1.amazonaws.com:5432/da78g96nvph3qq")
h.load_data_to_db
h.delete_old_data