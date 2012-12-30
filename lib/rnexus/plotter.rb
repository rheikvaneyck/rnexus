# This lib containts a module to read the database and generate
# images from that data.

module Rnexus
  class Plotter
    def initialize
    end
    def get_chart_data(type)
      data = DBManager::Measurement.all
      return data
    end
  end
end
