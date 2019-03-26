// Programa Java com RMI
// ShowBindings.java - Exibe os nomes dos servidores registrados
// Silvana Rossetto
//-------------------------------------------------------------


import java.rmi.*;
import java.rmi.server.*;


public class ShowBindings {

  // Metodo principal
  public static void main (String[] args) {
     try
     {
        String[] bindings = Naming.list("");
	for(int i=0; i<bindings.length; i++)
	   System.out.println(bindings[i]);
     } catch(Exception e) {
        System.out.println("ERRO_SHOWBindings " + e);
     }
  }
}
