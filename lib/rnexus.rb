$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))
require 'rnexus/options'
require 'rnexus/measurement'
require 'rnexus/plotter'
require 'rnexus/status'
require 'rnexus/runner'

module Rnexus
  VERSION = '0.3.0'
end
