#!/bin/env ruby
require 'yaml'
require 'erb'

APP_ROOT = File.join(File.dirname(__FILE__),'..','..') # HELPER

scheme_description = YAML.load_file(File.join(APP_ROOT,'config','scheme_description.yml'))
# puts scheme_description.inspect

File.open('create_migration.erb','r') do |f|
  erb = ERB.new( f.read )
  timestamp = Time.now.strftime('%Y%m%d%H%M%S') # HELPER
  File.open(File.join(APP_ROOT,'db','migrate',timestamp + '_create_measurements.rb'), 'w') do |e|
    e.write erb.result(binding)
  end
end
