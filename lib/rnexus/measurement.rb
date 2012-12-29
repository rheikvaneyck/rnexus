# This lib containts the database model
#--
# Copyright (c) 2011 Marcus Nasarek
# Licensed under the same terms as Ruby. No warranty is provided.

require 'active_record'
require 'yaml'
require 'logger'

module Rnexus
  class DBManager
    def initialize(db_path)
      db_config = YAML::load(File.open(File.join(db_path,'database.yml')))     
      ActiveRecord::Base.establish_connection(db_config)
      ActiveRecord::Base.logger = Logger.new(File.open(File.join(db_path,'database.log'), 'a'))
    end
    class Measurement < ActiveRecord::Base
      validates_uniqueness_of :DT # FIXME: make this more generell
    end
  end
end
