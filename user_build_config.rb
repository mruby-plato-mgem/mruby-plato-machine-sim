MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem '../mruby-plato-machine'
  conf.gem '../mruby-plato-machine-sim'

  enable_debug
  conf.enable_test
end
