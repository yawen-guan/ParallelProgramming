package checkcode.src.main.java.com.Gw.CheckService;

import java.io.IOException;

public class CheckApp{
    public static void main(String[] args) {
        Check check = new Check();
        try {
            check.Initial();
            check.outputResult();
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
