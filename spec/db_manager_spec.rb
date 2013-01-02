require 'rnexus'
require 'date'

describe Rnexus::DBManager do
  before(:all) do
      @db = Rnexus::DBManager.new('spec')
  end
  
  it "be the first col in the db and holds seconds from 1.1.1970" do
    timestamps = Rnexus::DBManager::Measurement.all.map(&:DT)
    datetimes =  timestamps.map { |t| Time.at(t).to_datetime }
    timestamps.first.is_a? Integer
    Date.valid_date?(datetimes.first.year, datetimes.first.month, datetimes.first.day)
  end
end
