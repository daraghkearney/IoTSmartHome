const char homepageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Smart Home Dashboard</title>
  <style>
    html {
      scroll-behavior: smooth;
    }
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    body {
      font-family: 'Arial', sans-serif;
      background: linear-gradient(135deg, #e0f7fa, #f1f8e9);
      color: #333;
      transition: background 0.5s ease;
    }
    .navbar {
      display: flex;
      justify-content: space-around;
      background-color: #333;
      padding: 1rem;
      position: sticky;
      top: 0;
      z-index: 1000;
    }
    .navbar a {
      color: white;
      text-decoration: none;
      font-size: 1.2rem;
      padding: 0.5rem 1rem;
    }
    .navbar a:hover {
      background-color: #555;
      border-radius: 5px;
    }
    .container {
      display: none;
      padding: 2rem;
      text-align: center;
      animation: fadeIn 0.8s ease-in-out;
    }
    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(20px); }
      to { opacity: 1; transform: translateY(0); }
    }
    #home {
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
      padding: 4rem 2rem;
      background: linear-gradient(rgba(255,255,255,0.85), rgba(255,255,255,0.85)),
                  url('smarthome.jpg') center/cover no-repeat;
      background-attachment: fixed;
    }
    #home h1,
    #home p {
      text-shadow: 0 1px 3px rgba(0, 0, 0, 0.2);
    }
    .thingspeak-graphs {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 1.5rem;
      margin-top: 2rem;
    }
    iframe {
      border: none;
      width: 450px;
      height: 260px;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      border-radius: 10px;
    }
    .control-buttons {
      display: flex;
      justify-content: center;
      gap: 1.5rem;
      margin-top: 2rem;
    }
    .control-buttons button {
      padding: 1rem 2rem;
      font-size: 1.2rem;
      border: none;
      background-color: #007BFF;
      color: white;
      border-radius: 5px;
      cursor: pointer;
      transition: background-color 0.3s ease, transform 0.2s ease;
    }
    .control-buttons button:hover {
      background-color: #0056b3;
      transform: scale(1.05);
    }
    .video-container {
      display: flex;
      justify-content: center;
      margin-top: 2rem;
    }
    footer {
      text-align: center;
      padding: 1rem;
      background-color: #989898;
      color: white;
      margin-top: 2rem;
      box-shadow: 0 -4px 6px rgba(0, 0, 0, 0.1);
    }
  </style>
</head>
<body>

  <nav class="navbar">
    <a href="#" onclick="showSection('home')">Home</a>
    <a href="#" onclick="showSection('charts')">Charts</a>
    <a href="#" onclick="showSection('control')">Control Center</a>
    <a href="#" onclick="showSection('about')">About My Project</a>
  </nav>

  <section id="home" class="container">
    <h1>Welcome to the IoT Smart Home Dashboard!</h1>
    <p>Monitor and control your smart home with ease.</p>
  </section>

  <section id="charts" class="container">
    <h2>Live Data Readings</h2>
    <div class="thingspeak-graphs">
      <iframe src="https://thingspeak.com/channels/2713003/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15" title="Temperature Graph"></iframe>
      <iframe src="https://thingspeak.com/channels/2713003/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15" title="Humidity Graph"></iframe>
      <iframe src="https://thingspeak.com/channels/2713003/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15" title="Motion Detection"></iframe>
    </div>
  </section>

  <section id="control" class="container">
    <h2>Control Center</h2>
    <p>Use the buttons below to control your smart home devices.</p>
    <div class="control-buttons">
      <button onclick="controlLED('on')">Turn Lights ON</button>
      <button onclick="controlLED('off')">Turn Lights OFF</button>
    </div>
  </section>

  <section id="about" class="container">
    <h2>About My Project</h2>
    <p>Watch the video below to learn more about my IoT Smart Home system.</p>
    <div class="video-container">
      <iframe width="560" height="315" src="https://www.youtube.com/watch?v=zgY56vgKHGU" 
              title="My project video" frameborder="0" 
              allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" 
              allowfullscreen>
      </iframe>
    </div>
  </section>

  <footer>
    <p>&copy; 2024 DK Smart Home System</p>
  </footer>

  <script>
    function showSection(sectionId) {
      document.querySelectorAll('.container').forEach(section => {
        section.style.display = 'none';
      });
      document.getElementById(sectionId).style.display = 'block';
    }
    window.onload = function() {
      showSection('home');
    };

    function controlLED(state) {
      fetch(`/led?state=${state}`)
        .then(res => res.text())
        .then(data => console.log("LED:", data))
        .catch(err => console.error("Fetch error:", err));
    }
  </script>
</body>
</html>
)rawliteral";
