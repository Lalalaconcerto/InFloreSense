live_loop :moisture do
  sample :bd_haus, rate: 1 #reflects moisture of plant
  #high moisture should be rate 3, low should be .5
  sleep 4 #Reflects chance of rain today (highest is 0.5, 1, 2, 4)
end

live_loop :temp do
  sample :ambi_choir, rate: 1 #reflects temperature of the plant
  sleep 1 #Reflects temperature outside (highest is 0.5, 1, 2, 4)
end
