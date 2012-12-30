require 'bundler/setup'
require 'sinatra'
require 'haml'
require 'rnexus'

get '/' do
  haml :index
end
