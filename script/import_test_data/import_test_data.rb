#!/bin/env ruby
require 'csv'
require 'active_record'

APP_ROOT = File.join(File.dirname(__FILE__),'..','..') # HELPER

scheme_description = YAML.load_file(File.join(APP_ROOT,'db','scheme_description.yml'))
data = CSV.read('weatherAll.data', { col_sep: ':' })

ActiveRecord::Base.establish_connection(YAML::load(File.open(File.join(APP_ROOT,'db','database.yml'))))

class Measurement < ActiveRecord::Base
  
end

data.each do |d|
  # Measurement.create
end
