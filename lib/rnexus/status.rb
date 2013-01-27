# This lib containts a module to read the database and generate
# images from that data.
require 'date'

module Rnexus
  class Status
    def initialize(db_config)
      @db = Rnexus::DBManager.new(db_config)
    end
    def get_last_state()
      # returns Status Object
      state = Rnexus::DBManager::State.order("created_at DESC").first
      return state
    end
  end    
end
