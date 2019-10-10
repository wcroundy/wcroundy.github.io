//our username
var name;
var connectedUser;
//web document variables
var loginPage = document.querySelector('#loginPage');
var usernameInput = document.querySelector('#usernameInput');
var loginButton = document.querySelector('#loginButton');
var callPage = document.querySelector('#callPage');
var callToUsernameInput = document.querySelector('#callToUsernameInput');
var callButton = document.querySelector('#callButton');
var hangUpButton = document.querySelector('#hangUpButton');
var localVideo = document.querySelector('#localVideo');
var remoteVideo = document.querySelector('#remoteVideo');
var yourConn;
var stream;
//connect to our signaling server using websocket protocol
var signalingServer = new WebSocket('ws://161.28.13.135:9090');
//when the connection to our signaling server opens, the server displays this message
signalingServer.onopen = function ()
{
   console.log("Connected to the signaling server");
};
//when we get a message from a signaling server
signalingServer.onmessage = function (msg)
{
   //display when reveived message and data from signaling server
   console.log("Got message", msg.data);
   //variable data holds the string parsed from the JSON object sent form signaling server
   var data = JSON.parse(msg.data);
   //Handle our cases for various interactions with the signaling server
   switch(data.type)
   {
      case "login":
         handleLogin(data.success);
         break;
      case "offer":
         handleOffer(data.offer, data.name);
         break;
      case "answer":
         handleAnswer(data.answer);
         break;
      case "candidate":
         handleCandidate(data.candidate);
         break;
      case "leave":
         handleLeave();
         break;
      default:
         break;
   }
};
//function to log an error when  connection fails
signalingServer.onerror = function (err)
{
   console.log("Got error", err);
};
//function to change string to JSON format and send as message
function send(message)
{
   //concatenate remote peer username to message
   if (connectedUser)
   {
      message.name = connectedUser;
   }
   //format message as JSON and send
   signalingServer.send(JSON.stringify(message));
};

//**************************
//Switch Handling Functions
//**************************
//hide the callPage
callPage.style.display = "none";
// Login when the user clicks the button
loginButton.addEventListener("click", function (event)
{
   name = usernameInput.value;
   if (name.length > 0)
   {
      send
   (
      {
         type: "login",
         name: name
      }
      );//end send
   }
}
);//end loginButton
//hang up
hangUpButton.addEventListener("click", function ()
{
   send({type: "leave"});
   handleLeave();
});//end hangup button
//initiating a call
callButton.addEventListener("click", function ()
{
   var callToUsername = callToUsernameInput.value;
   if (callToUsername.length > 0)
   {
      connectedUser = callToUsername;
      // create an offer
      yourConn.createOffer(function (offer)
      {
       send({type: "offer",offer: offer});
       yourConn.setLocalDescription(offer);
      }, function (error)
         {
          alert("Error when creating an offer");
         }
     );//end yourConn.createOffer
   }
});
//**************************
//Switch Handling Functions
//**************************

//Function when a user logs into the site
function handleLogin(success)
{
   if (success === false)
   {
      alert("Ooops...try a different username");
   }
   else if(navigator.userAgent.indexOf("Firefox") != -1 )
   {
      loginPage.style.display = "none";
      callPage.style.display = "block";
      //**********************
      //Starting a peer connection
      //**********************
   
      //getting local video stream
      navigator.mozGetUserMedia({ video: true, audio: true }, function (myStream)
      {
        //displaying local video stream on the page
  localVideo.srcObject = myStream;
        //using Google public stun server
        var ICE_Configuration = {"iceServers": [{ "url": "stun:stun2.1.google.com:19302" }]};
        yourConn = new mozRTCPeerConnection(ICE_Configuration);
        //display your local stream
        yourConn.addStream(myStream);
  //On the Event of call accepted 
        yourConn.onicecandidate = function (event)
        {
            if (event.candidate)
            {
               send({type: "candidate",candidate: event.candidate});
            }
        };
        //display remote user stream
  yourConn.onaddstream = function (e) {remoteVideo.srcObject = e.stream;};

        }, function (error){console.log(error);}
       );//end navigator.mozGetUserMedia
   }//end else statemtn
   else if(navigator.userAgent.indexOf("Chrome") != -1 )
   {
      loginPage.style.display = "none";
      callPage.style.display = "block";
      //**********************
      //Starting a peer connection
      //**********************
      //getting local video stream
      navigator.webkitGetUserMedia({ video: true }, function (myStream)
      {
        //displaying local video stream on the page
  localVideo.srcObject = myStream;
        //using Google public stun server
        var ICE_Configuration = {"iceServers": [{ "url": "stun:stun2.1.google.com:19302" }]};
        yourConn = new webkitRTCPeerConnection(ICE_Configuration);
   
  //On the Event of call accepted 
         yourConn.onicecandidate = function (event)
         {
            if (event.candidate)
            {
               send({type: "candidate",candidate: event.candidate});
            }
         };
         yourConn.addStream(myStream);
         //display remote user stream
   yourConn.onaddstream = function (e) {remoteVideo.srcObject = e.stream;};
        }, function (error){console.log(error);}
       );//end navigator.mozGetUserMedia
   }
};
//when somebody sends us an offer
function handleOffer(offer, name)
{
   connectedUser = name;
   yourConn.setRemoteDescription(new RTCSessionDescription(offer));
   //create an answer to an offer
   yourConn.createAnswer(function (answer)
   {
      yourConn.setLocalDescription(answer);
      send({type: "answer",answer: answer});
   }, function (error)
      {
       alert("Error when creating an answer");
      }
   );//end yourConn.createAnswer
};
//when we get an answer from a remote user
function handleAnswer(answer)
{
   yourConn.setRemoteDescription(new RTCSessionDescription(answer));
};
//when we get an ice candidate from a remote user
function handleCandidate(candidate)
{
   yourConn.addIceCandidate(new RTCIceCandidate(candidate));
};
function handleLeave()
{
   connectedUser = null;
   remoteVideo.src = null;
   yourConn.close();
   yourConn.onicecandidate = null;
   yourConn.onaddstream = null;
};// end of handling leave event