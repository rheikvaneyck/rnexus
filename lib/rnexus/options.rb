#!/usr/bin/env ruby
require 'optparse'

module Rnexus
  class Options
    BASE_DIR = "rnexus"
    DATABASE_DIR = "db"
    DATABASE_FILE = "weather_data.sqlite3"   
    
    attr_reader :base_dir
    attr_reader :database_dir
    attr_reader :database_file
    
    def initialize(argv)
      @base_dir = BASE_DIR
      @database_dir = DATABASE_DIR
      @database_file = DATABASE_FILE
      
      parse(argv)
      puts argv
    end
    private
    def parse(argv)
      OptionParser.new do |opts|
        opts.banner = "Usage: rnexus [ options ]"
        opts.on("-b", "--base-dir path", String, "Path to directory") do |b|
          @base_dir = b
        end
        opts.on("-d", "--db-dir path", String, "Path to directory") do |db|
          @database_dir = db
        end
        opts.on("-f", "--db-file name", String, "File name") do |f|
          @database_file = f
        end
        opts.on("-h", "--help", "Show this message") do
          puts opts
          exit
        end
        begin
          opts.parse!(argv)
        rescue OptionParser::ParseError => e
          STDERR.puts e.message, "\n", opts
          exit(-1)
        end        
      end
    end
  end
end