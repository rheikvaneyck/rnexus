require 'active_record'
require 'yaml'
require 'logger'

task :default => "orga:show_todos"

namespace :orga do
  desc "ToDos"
  task :show_todos do
    puts File.read('TODOS.TXT')
  end 
end

namespace :db do
  desc "Migrate the database through scripts in db/migrate. Target specific version with VERSION=x"
  task :migrate => :environment do
    ActiveRecord::Migrator.migrate('db/migrate', ENV["VERSION"] ? ENV["VERSION"].to_i : nil )
  end
  
  task :environment do
    ActiveRecord::Base.establish_connection(YAML::load(File.open(File.join('db','database.yml'))))
    ActiveRecord::Base.logger = Logger.new(File.open(File.join('db','database.log'), 'a'))
  end  
end
