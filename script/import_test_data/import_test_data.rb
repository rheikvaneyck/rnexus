#!/bin/env ruby
require 'csv'
require 'active_record'

APP_ROOT = File.join(File.dirname(__FILE__),'..','..') # HELPER

scheme_description = YAML.load_file(File.join(APP_ROOT,'db','scheme_description.yml'))
data = CSV.read('weatherAll.data', { col_sep: ':' })

db_config = YAML::load(File.open(File.join(APP_ROOT,'db','database.yml')))

ActiveRecord::Base.establish_connection(db_config)

class Measurement < ActiveRecord::Base
  validates_uniqueness_of :DT # FIXME: make this more generell
end

data.each do |d|
  i = 0
  scheme_description.each do |key, value|
    scheme_description[key] = d[i] unless d[i] == "i"
    i += 1
  end
  
  Measurement.create scheme_description
end
