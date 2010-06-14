require "rubygems"
require "test_benchmark"

require "test/unit"

# $:.unshift File.dirname(__FILE__) + "/../ext/phenomatrix"
require "fastknn"

class TestPhenomatrixExtn < Test::Unit::TestCase
  def setup
    @@pair ||= Fastknn::PhenomatrixPair.new(185, 3)
  end

  def test_truth
    assert true
  end

end
