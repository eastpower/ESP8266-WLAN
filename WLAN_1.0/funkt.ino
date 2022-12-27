void antOn(byte a) {
  for (byte i = 0; i <= 4; i++) {
    if (i == a) {
      An(a);
    } else {
      client.print("<p><a href='/ant");
      client.print(i);
      client.print("'><button type='button'>");
      client.print(text[i]);
      client.println("</button></a></p>");
    }
  }
}

void An(byte a) {
  client.print("<p><button type='button' class='on'>");
  client.print(text[a]);
  client.println("</button></p>");
  for (byte i = 0; i <= 4; i++) {
    if (i == a) {
      digitalWrite(ant[i], HIGH);
    } else {
      digitalWrite(ant[i], LOW);
    }
  }
}