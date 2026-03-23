require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/test_*.rb"]
end

if RUBY_ENGINE == "truffleruby"
  task :compile do
    # noop
  end

  task :clean do
    # noop
  end
else
  require "rake/extensiontask"

  Rake::ExtensionTask.new("stackprof") do |ext|
    ext.ext_dir = "ext/stackprof"
    ext.lib_dir = "lib/stackprof"
  end
end

task :compile_for_test do
  ENV['STACKPROF_TESTING'] = '1'
  Rake::Task[:clean].invoke
  Rake::Task[:compile].invoke
end

task test: :compile_for_test
task default: :test
