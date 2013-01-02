# This lib containts a module to read the database and generate
# images from that data.

module Rnexus
  class Plotter
    def initialize
      @db = Rnexus::DBManager.new('config') # FIXME: make this from a variable
    end
    def get_chart_data(type)
      data = Rnexus::DBManager::Measurement.all
      return data
    end
  end
end
