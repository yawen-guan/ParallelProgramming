package checkcode.src.main.java.com.Gw.CheckService;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Check {
    private final int MaxGlobal = 1000;
    private final int MaxFunc = 1000;
    private final int MaxFuncLine = 1000;
    private String[] global;
    private String[] func;
    private boolean[] isConst;
    private boolean[] isReentrant;
    private int globalId;
    private int funcId;
    private String[] funcLine;
    private int funcLineCnt;

    Check() {
        global = new String[MaxGlobal];
        isConst = new boolean[MaxGlobal];
        globalId = 0;
        func = new String[MaxFunc];
        isReentrant = new boolean[MaxFunc];
        funcId = 0;
        funcLine = new String[MaxFuncLine];
        funcLineCnt = 0;
    }

    public void Initial() throws IOException {
        BufferedReader scanner = new BufferedReader(new InputStreamReader(System.in));
        try {
            String line = scanner.readLine();
            while (line != null) {
                if (line.startsWith("@"))
                    handleGlobal(line);
                else {
                    if (line.startsWith("define") == true) {
                        funcLineCnt = 1;
                        funcLine[0] = line;

                        while (true) {
                            line = scanner.readLine();
                            funcLine[funcLineCnt++] = line;
                            if (line.charAt(0) == '}')
                                break;
                        }
                        handleFunction();
                    }
                }
                line = scanner.readLine();

            }
        } catch (IOException e) {
            throw e;
        }
    }

    public void handleGlobal(String line) {
        String[] words = line.split("\\s");
        global[globalId] = words[0];
        isConst[globalId] = false;

        boolean pre = false;
        for (String word : words) {
            if (pre == true && word.compareTo("constant") == 0) {
                isConst[globalId] = true;
                break;
            }
            if (word.compareTo("dso_local") == 0 || word.compareTo("dso_preemptable") == 0) {
                pre = true;
            } else
                pre = false;
        }
        globalId++;
    }

    public String findFuncName(String line) {
        String[] words = line.split("(\\(|,|\\s)");
        for (String word : words) {
            if (word.startsWith("@"))
                return word;
        }
        return null;
    }

    public int isGlobal(String word) { // 0: no 1: const 2: variable
        for (int i = 0; i < globalId; i++) {
            if (word.compareTo(global[i]) == 0) {
                if (isConst[i] == true)
                    return 1;
                else
                    return 2;
            }
        }
        return 0;
    }

    public int findFuncId(String word) {
        for (int i = 0; i < funcId; i++) {
            if (word.compareTo(func[i]) == 0) {
                return i;
            }
        }
        return -1;
    }

    public boolean handleFunctionLine(String line) {// true:re false:non-re
        String[] words = line.split("(\\(|,|\\s)");
        boolean ret = false;
        for (String word : words) {
            if (word.compareTo("ret") == 0)
                ret = true;
            if (word.compareTo("@malloc") == 0)
                return false;
            if (word.startsWith("@")) {
                int res = isGlobal(word);
                if (res == 2)
                    return false;
                if (res == 1 && ret == true)
                    return false;
                if (res == 0) {
                    int id = findFuncId(word);
                    if (id != -1 && isReentrant[id] == false)
                        return false;
                }
            }
        }
        return true;
    }

    public void handleFunction() {
        func[funcId] = findFuncName(funcLine[0]);
        isReentrant[funcId] = true;

        for (int i = 0; i < funcLineCnt; i++) {
            if (handleFunctionLine(funcLine[i]) == false) {
                isReentrant[funcId] = false;
                break;
            }
        }
        funcId++;
    }

    public void outputResult() {
        int cntRe = 0;
        for (int i = 0; i < funcId; i++) {
            String name = func[i].substring(1);
            if (isReentrant[i] == true) {
                cntRe++;
                System.out.println(name + " is reentrant");
            } else {
                System.out.println(name + " isn't reentrant");
            }
        }
        System.out.println("");
        System.out.println(cntRe + " functions are reentrant, " + (funcId - cntRe) + " functions are not");
    }
}