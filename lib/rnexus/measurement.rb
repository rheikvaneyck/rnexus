# This lib containts the database model
#--
# Copyright (c) 2011 Marcus Nasarek
# Licensed under the same terms as Ruby. No warranty is provided.

require 'active_record'
require 'uri'
require 'yaml'
require 'logger'

module Rnexus
  class DBManager
    def initialize(config_path)
      if (ENV['DATABASE_URL']) then
        db = URI.parse(ENV['DATABASE_URL'] || 'postgres://localhost/app-dev')
        db_config = {
          :adapter  => db.scheme == 'postgres' ? 'postgresql' : db.scheme,
          :host     => db.host,
          :port     => db.port,
          :username => db.user,
          :password => db.password,
          :database => db.path[1..-1],
          :encoding => 'utf8',
          :log_dir => "log"
        }
      else
        db_config = YAML::load(File.open(File.join(config_path,'database.yml')))
        # log_dir = db_config["log_dir"]
        # ActiveRecord::Base.logger = Logger.new(File.open(File.join(log_dir,'database.log'), 'a'))
      end

      ActiveRecord::Base.establish_connection(db_config)
    end
    class Measurement < ActiveRecord::Base
      validates_uniqueness_of :DT # FIXME: make this more generell
    end
    class State < ActiveRecord::Base
    end
  end
end
