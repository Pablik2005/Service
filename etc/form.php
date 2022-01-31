<html>
<head>
	<meta charset="utf-8" >
</head>
<body>

<?php

class SQL
{
	protected $servername;
	protected $username; 
	protected $password; 
	protected $dbname;
	protected $con;
	
	public function __construct()
	{
		$this->servername = "192.168.3.206";
		$this->username = "service";
		$this->password = "service26062014!";
		$this->dbname = "service";
	}
		
	protected function Login(){
		$this->con = mysqli_connect($this->servername, $this->username, $this->password, $this->dbname);
		if (!$this->con) {
			die("Connection failed: " . mysqli_connect_error());
		}
		if (mysqli_connect_errno()) {
			die("Connect failed: %s\n".mysqli_connect_error());
		}
		$this->con->set_charset("utf8");
	}

	protected function Logout(){
		$this->con->close();
	}	
	
	public function Select($query){
		$this->Login();
		$result = $this->con->query($query);
		$this->Logout();
	
		return $result;
	}
	

}
$sql = new SQL();

$orders = $sql->Select("SELECT * FROM orders WHERE orderID=".$_GET["id"]);
$client;

if($orders->num_rows > 0)
{
	$order = $orders->fetch_assoc();
	
	$clients = $sql->Select("SELECT * FROM clients WHERE peselNIP=".$order["peselNIP"]);
	if($clients->num_rows > 0)	
		$client = $clients->fetch_assoc();

}


?>


<style>
	#moxum{
		font-family: "Times New Roman";
		font-size: 35px;
		text-align: center;
		font-weight: bold;
	}
	
	#title{
		font-family: "Times New Roman";
		font-size: 20px;
		text-align: center;
		font-weight: bold;
	}
	
	#personalData{
		float: left;
		width: 350px;
	}
	
	#deviceInfo{
		float: left;
		width: 350px;
	}
	
	#personalDataTitle{
		float: left;
	}
	
	#personalDataValue{
		float: left;
		text-align: center;
		font-weight: bold;
		width: 235px;
	}
	
	#deviceInfoTitle{
		float: left;
	}
	
	#deviceInfoValue{
		float: left;
		text-align: center;
		font-weight: bold;
		width: 265px;
	}
	
</style>

	<div id="moxum">
		MOXUM
	</div>
	<div id="title">
		Formularz zgłoszeniowy
	</div>
	
	<div id="orderID">
		ID: <b><?php  echo $order["orderID"]; ?></b>
	</div>
	<div id="recivedDate">
		Data przyjęcia: <b><?php  echo $order["receivedDate"]; ?></b>
	</div>
	
	<br><hr><br>
	
	<div id="personalData">
		<center><b><font size="4">Dane Osobowe:</font></b></center><br>
		<div id="personalDataTitle">
			PESEL / NIP: <br>
			Imię / Nazwa: <br>
			Adres: <br>
			Poczta: <br>
			Email: <br>
			Tel. Stacjonarny: <br>
			Tel. Komórkowy<br>
		</div>
		<div id="personalDataValue">
			<?php  echo $client["peselNIP"]; ?><br>
			<?php  
				if($client["isCompany"] == 1)
					echo $client["companyName"]; 
				else 
					echo $client["name"]." ".$client["surName"];
			?><br>
			<?php  echo $client["address"]; ?><br>
			<?php  echo $client["postOffice"]; ?><br>
			<?php  echo $client["email"]; ?><br>
			<?php  echo $client["landlinePhone"]; ?><br>
			<?php  echo $client["mobilePhone"]; ?>
		</div>
	</div>
	<div id="deviceInfo">
		<center><b><font size="4">Informacje o urządzeniu:</font></b></center><br>
		<div id="deviceInfoTitle">
			Urządzenie:<br>
			Producent:<br>
			Model: <br>
			SN:<br>
			Hasło:<br>
			Dodatkowo :<br>
			Opis:<br>
		</div>
		<div id="deviceInfoValue">
			<?php  echo $order["deviceType"]; ?><br>
			<?php  echo $order["deviceProducer"]; ?><br>
			<?php  echo $order["deviceModel"]; ?><br>
			<?php  echo $order["deviceSN"]; ?><br>
			<?php  echo $order["password"]; ?><br>
			<?php  echo $order["additionalEquipment"]; ?><br>
			<?php  echo $order["description"]; ?>
		</div>
	</div>
	<div style="clear: both">
	
	<br><hr><br>
	
	<div id="settlement">
	Rozliczenie:<br>
		<?php
			
			if($order["invoice"] == 1)
			{
				echo '
				&emsp;&emsp; Faktura <input type="checkbox" checked><br>
				&emsp;&emsp; Paragon <input type="checkbox"><br>
				';

			}else
			{
				echo '
				&emsp;&emsp; Faktura <input type="checkbox" ><br>
				&emsp;&emsp; Paragon <input type="checkbox" checked><br>
				';
			}
		?>
	<div>
	
	<br><hr><br>
	
	<div id="terms">
		<center>Regulamin:</center><br><br>
		
		1.	Podpisując regulamin klient oświadcza że zapoznał się z zasadami regulaminu i dobrowolnie je akceptuje.<br>
		2.	Klient oświadcza, że posiada wymagane licencje do programów zainstalowanych na swoim sprzęcie.<br>
		3.	Warunkiem przyjęcia przez serwis zlecenia wykonania usługi jest dostarczenie urządzenia do serwisu i podanie niezbędnych danych osobowych.<br>
		4.	Pokwitowanie serwisowe jest jedynym dokumentem pozwalającym na odbiór sprzętu z serwisu i musi ono zostać zwrócone przy odbiorze. W przypadku zagubienia pokwitowania jedyną osobą mogącą odebrać sprzęt jest ta na którą było rejestrowane zlecenie.<br>
		5.	Koszt diagnozy urządzenie wynosi od 20 do 30 zł w zależności od czasu poświęconego na diagnozę.<br>
		6.	W przypadku kiedy Klient nie odbierze urządzenia po upływie 1 miesiąca od dnia poinformowania go o odbiorze  mogą  zostać naliczana opłata magazynowa w wysokości 3,5 zł dziennie. Brak możliwości zawiadomienia klienta w sposób przez niego wskazany, a nie wynikający z błędu serwisu, nie zwalnia klienta z opłaty magazynowej. W przypadku naliczenia opłaty magazynowej, odbiór sprzętu nastąpi po uregulowaniu tej opłaty na rzecz serwisu.<br>
		7.	Po upływie 3 miesięcy,  nieodebrany sprzęt ulega utylizacji i jest uznawany jako porzucony przez właściciela.<br>
		8.	Urządzenie podlega gwarancji gdy nie upłynął termin gwarancji a plomba gwarancyjna jest nienaruszona.<br>
		9.	Wymienione części nie podlegają zwrotowi, wyjątkiem są sytuacje uzgodnione przed przyjęciem sprzętu.<br>

		<br><br><br><br><br><br><br>
		............................<br>
		Podpis klienta
		
		
		
	</div>

</body>
</html>