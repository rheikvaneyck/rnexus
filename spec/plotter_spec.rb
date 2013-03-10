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
    Time.at(measurement.DT).to_datetime.should eq("Sun, 10 Mar 2013 13:17:02 +0100")
  end
  it "get_last_24h returns col DT and here col PRESS for last 24h" do
    data = @plotter.get_last_24h(:PRESS)
    data.length.should eq 25
    data.last.should eq ["2013-03-10 12:17:02", 992.3]
  end
  it "get_day_min_max returns per date here the min and max temp" do
    dmm = @plotter.get_days_min_max(7, :T5)
    dmm.length.should eq 8
    dmm.last.should eq ["2013-03-10", -1.5, -0.1]
  end
end
