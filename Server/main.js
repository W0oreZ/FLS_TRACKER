const mqtt = require('mqtt')
const axios = require('axios')
const moment = require('moment')

//MQTT Connection
const client  = mqtt.connect('mqtt://41.140.242.182:1338',{clientId:"EXP_GEO_RELAI"})


client.on('connect', function () {
  console.log("Connected to EXP NETWORK");

  client.subscribe('exp/FLS/data');
});


client.on('message', function (topic, message) {

  let data;
  context = message.toString();
  context = context.replace(/[^ -~]+/g, "");
  let TrackerData = false;
  try {
    data = JSON.parse(context);
    data.dt = moment(Date.now()).format('YYYY-MM-DD HH:mm:ss');
    TrackerData = {
      imei: data.imei,
      lat: data.lat,
      lng: data.lng,
      altitude: data.alt,
      angle: 0,
      speed: data.speed,
      params:`ai1=${data.ai1}|`,
      dt_tracker: data.dt,
      net_protocol:"FLS",
      protocol:"FLS_RELAI",
      loc_valid:1,
      event:"",
      op:"loc"
    };
    
    console.log(TrackerData);
  } catch (error) {
    console.log(error.message)
  }
  //context.split(",").map(function (val) {
  //  data.push(val);
  //});
//
  if(TrackerData)
  {
    axios
    .get('https://geo16.net/api/api_loc.php',{params:TrackerData})
    .then((response) =>{
      console.log(moment().format('YYYY-MM-DD HH:mm:ss')+' - Packet Transmition '+response.data);
    })
    .catch(error=>console.log(error.message));
  }

  
  //
  //
  //
  //
  //
  //

});