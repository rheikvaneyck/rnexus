require 'rnexus'
require 'date'

describe Rnexus::Plotter do
  before(:all) do
      @plotter = Rnexus::Plotter.new('spec')
  end
  
  it "get_chart_data by default returns col :DT in the db and holds seconds from 1.1.1970" do
    timestamps = @plotter.get_chart_data
    timestamps.first.is_a? Integer
  end
  it "get_chart_data by default returns seconds from 1.1.1970" do
    timestamps = @plotter.get_chart_data
    datetimes =  timestamps.map { |t| Time.at(t).to_datetime }    
    Date.valid_date?(datetimes.first.year, datetimes.first.month, datetimes.first.day)
  end
  it "get_last_measurement returns newest measurement" do
    measurement = @plotter.get_last_measurement
    Time.at(measurement.DT).to_datetime.should eq("2012-12-28T19:16:00+01:00")
  end
  it "get_last_24h returns col DT and here col PRESS for last 24h" do
    data = @plotter.get_last_24h(:PRESS)
    data.length.should eq 25
    data.last.should eq ["2012-12-28 18:16:00", 1021.0]
  end
  it "get_day_min_max returns per date here the min and max temp" do
    dmm = @plotter.get_days_min_max(:T0)
    dmm.length.should eq 5
    dmm.last.should eq ["2012-12-28", 19.9, 22.6]
  end
end
