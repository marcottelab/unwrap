# File: Rakefile

require 'rubygems'
require 'rake/extensiontask'
require 'rake/testtask'
require 'hoe'

PKG = "fastknn"
PKG_VERSION = [0,0,4]
AUTHOR = "John O. Woods, Marcotte Lab"
EMAIL = "john.woods@marcottelab.org"
HOMEPAGE = "http://github.com/MarcotteLabGit/fastknn"

Hoe.plugin :git
Hoe.plugin :debugging
Hoe.plugin :test

Hoe.spec PKG do |p|
  p.developer AUTHOR, EMAIL

  p.readme_file    = "README.rdoc"

  p.extra_deps << ["hoe", ">= 2.2.0"]
  p.extra_deps << ["test_benchmark", ">= 0.4.7"]
  p.extra_deps << ["rice", ">= 1.3.2"]
end

Rake::ExtensionTask.new('distance_matrix')

namespace :test do
  Rake::TestTask.new(:phenomatrix_pair) do |t|
    t.test_files = FileList['test/test_phenomatrix_pair_extn.rb']
    t.warning = true
    t.verbose = true
  end
end
