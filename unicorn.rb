require 'fileutils'
# set path to app that will be used to configure unicorn, 
# note the trailing slash in this example
@dir = File.expand_path(File.dirname(__FILE__))

worker_processes 2
working_directory @dir

timeout 30

# Specify path to socket unicorn listens to, 
# we will use this in our nginx.conf later
FileUtils.mkdir_p("#{@dir}/tmp/sockets") unless Dir.exists?("#{@dir}/tmp/sockets")
listen "#{@dir}/tmp/sockets/unicorn.sock", :backlog => 64
listen 8080, :tcp_nopush => true

# Set process id path
FileUtils.mkdir_p("#{@dir}/tmp/pids") unless Dir.exists?("#{@dir}/tmp/pids")
pid "#{@dir}/tmp/pids/unicorn.pid"

# Set log file paths
FileUtils.mkdir_p("#{@dir}/log") unless Dir.exists?("#{@dir}/log")
stderr_path "#{@dir}/log/unicorn.stderr.log"
stdout_path "#{@dir}/log/unicorn.stdout.log"                                        