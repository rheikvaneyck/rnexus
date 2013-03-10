require 'csv'
require 'active_record'
require 'yaml'
require 'logger'
require 'rake'
require 'rspec/core/rake_task'

task :default => "orga:show_todos"

namespace :orga do
  desc "ToDos"
  task :show_todos do
    puts File.read('TODOS.TXT')
  end 
end

namespace :web do
  desc "Run the sinatra app"
  task :run do
    ruby "-Ilib web/web_rnexus.rb"
  end
end

namespace :db do
  desc "Migrate the database through scripts in db/migrate. Target specific version with VERSION=x"
  task :migrate => :environment do
    ActiveRecord::Migrator.migrate('db/migrate', ENV["VERSION"] ? ENV["VERSION"].to_i : nil )
  end
  
  desc "Fill the database with data"
  task :load_data => :environment do
    scheme_description = YAML.load_file(File.join('config','scheme_description.yml'))
    data = CSV.read(File.join('data','weather.data'), { col_sep: ':' })
    
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

  end

  desc "Fill the database with states"
  task :load_states => :environment do
    scheme_description = YAML.load_file(File.join('config','states_description.yml'))
    data = CSV.read(File.join('data','weather.status'), { col_sep: ':' })
    
    class State < ActiveRecord::Base
    end
    
    data.each do |d|
      i = 0
      scheme_description.each do |key, value|
        scheme_description[key] = d[i] unless d[i] == "i"
        i += 1
      end
      
      State.create scheme_description
    end

  end
  
  task :environment do
    ActiveRecord::Base.establish_connection(YAML::load(File.open(File.join('config','database.yml'))))
    ActiveRecord::Base.logger = Logger.new(File.open(File.join('log','database.log'), 'a'))
  end  
end

namespace :test do
  desc "run all specs" 
  RSpec::Core::RakeTask.new(:spec) do |t|
    t.pattern =  'spec/*_spec.rb'
  end
end
