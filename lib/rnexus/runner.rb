#!/usr/bin/env ruby
# This module encaspulates functionality to run the weather analyzer programm.
# The main programm flow is in the run method
#--
# Copyright (c) 2011 Marcus Nasarek
# Licensed under the same terms as Ruby. No warranty is provided.

module Rnexus
  class Runner
    # Initilialize the runner. Heaps of options are defined, parsed
    # with the Rnexus::Options class. During initialization
    # the folder tree holding the data for the new server is created
    # with create_directories. Templates for the credential file are
    # copied to specific locations in the new folder tree. Furthermore
    # a connection to the sqlite3 database is established.
    def initialize(argv)
      @options = Rnexus::Options.new(argv)
      @db = Rnexus::DBManager.new(File.join(@options.base_dir,@options.database_dir, @options.database_file))
    end
    
    def run
    end
  end
end