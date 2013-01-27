#!/bin/env ruby
require 'yaml'
require 'erb'

APP_ROOT = File.join(File.dirname(__FILE__),'..','..') # HELPER

states_description = YAML.load_file(File.join(APP_ROOT,'config','states_description.yml'))
# puts states_description.inspect

File.open('create_states_migration.erb','r') do |f|
  erb = ERB.new( f.read )
  timestamp = Time.now.strftime('%Y%m%d%H%M%S') # HELPER
  File.open(File.join(APP_ROOT,'db','migrate',timestamp + '_create_states.rb'), 'w') do |e|
    e.write erb.result(binding)
  end
end
