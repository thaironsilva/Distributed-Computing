// Programa Java com RMI
// PageClient.java - Classe usuaria de paginas
//-------------------------------------------------------------

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class PageClient {
  private PageClient() {}
  // Metodo principal
  public static void main (String[] args) {
    double pagina;
    double n1, n2;
    String c;
    Scanner scanner = new Scanner(System.in);

    // Especifica o nome do servidor e do objeto para obter um stub para acessar o objeto servidor
    String host = (args.length < 1) ? null : args[0];
    try {
       Registry registry = LocateRegistry.getRegistry(host);
       Page stub = (Page) registry.lookup("Page");
       for(;;) {
          // Requisita uma pagina
          n1 = Double.parseDouble(scanner.nextLine());
          c = scanner.nextLine();
          n2 = Double.parseDouble(scanner.nextLine());
          pagina = stub.request(n1, c.charAt(0), n2);
          System.out.println(pagina);
       }
    } catch (Exception e) {
       System.err.println("Erro cliente: " + e.toString());
       e.printStackTrace();
    }
  }
}
