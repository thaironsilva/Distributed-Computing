// Programa Java com RMI
// Page.java - Interface compartilhada entre cliente e servidor
//-------------------------------------------------------------

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Page extends Remote {

  // Requisicao de uma pagina
  public double request (double n1, char c, double n2) throws RemoteException;

  // Liberaracao de uma pagina
  public void release (int id) throws RemoteException;
}
