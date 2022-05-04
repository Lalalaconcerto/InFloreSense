$temperature = 0
$moisture = 0
$onOff = 0

def plant_data_usable(d)
  reduced = d
  #find instance of first comma
  #create substring leading up to first comma
  commaI = reduced.index(',')
  
  moistureInput = Integer(reduced[0..commaI - 1])
  reduced = reduced[commaI + 1..-1]
  commaI = reduced.index(',')
  temperatureInput = Integer(reduced[0..commaI - 1])
  reduced = reduced[commaI + 1..-1]
  commaI = reduced.index(',')
  $onOff = Integer(reduced[0..commaI - 1])
  if temperatureInput < 0
    temperatureInput = 0
  end
  $moisture = 14.83926 + (0.5 - 14.83926) / (1 + (moistureInput/37.12209)**(7.613421))
  #if $moisture < 24
  #  $moisture = 0.5
  #elsif $moisture < 28
  #  $moisture = 1
  #elsif $moisture < 32
#    $moisture = 2
#  else
#    $moisture = 4
#  end

  $temperature = 256.2785 + (0.5 - 256.2785) / (1 + (temperatureInput/76.32976)**3.832485)
#  if $temperature < 15
#    $temperature = 0.5
#  elsif $temperature < 20
#    $temperature = 1.0
#  elsif $temperature < 25
#    $temperature = 2.0
#  else
#    $temperature = 4.0
#  end
end

File.open('/home/pi/Desktop/test.txt') do |file|
  file.each_line {
    |line|
    plant_data_usable(line)
    
  }
  
end

if $onOff == 1
  puts "#$moisture"
  puts "#$temperature"
  1.times do
    sample :bd_haus, rate: $temperature
    sample :ambi_choir, rate: $moisture
    sleep 1
  end
end
