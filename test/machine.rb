assert("PlatoSim::Machine.millis") do
  assert_equal Plato::Machine.micros.class, Fixnum
end

assert("PlatoSim::Machine.micros") do
  assert_equal Plato::Machine.micros.class, Fixnum
end

assert("PlatoSim::Machine.delay") do
  assert_raise(TypeError) {Plato::Machine.delay('1')}

  WAIT = 500
  RANGE = 10
  ms = Plato::Machine.millis
  Plato::Machine.delay WAIT
  dt = Plato::Machine.millis - ms - WAIT
  assert_equal 0, (dt / RANGE).to_i * RANGE
end

assert("PlatoSim::Machine.delay_us") do
  assert_raise(TypeError) {Plato::Machine.delay_us('1')}

  WAIT = 500 * 1000
  RANGE = 10 * 1000
  us = Plato::Machine.micros
  Plato::Machine.delay_us WAIT
  dt = Plato::Machine.micros - us - WAIT
  assert_equal 0, (dt / RANGE).to_i * RANGE
end
