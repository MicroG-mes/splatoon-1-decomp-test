// Import Splatoon 1 (Gambit) Symbols from tools/symbols.txt
// @category SplatoonDecomp
// @author SplatoonDecomp Team

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolTable;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class ImportSplatoonSymbols extends GhidraScript {

    @Override
    public void run() throws Exception {
        File file = new File("C:/Users/manel/Documents/splatoondecomp/tools/symbols.txt");
        if (!file.exists()) {
            println("[-] Symbols file not found: " + file.getAbsolutePath());
            return;
        }

        SymbolTable st = currentProgram.getSymbolTable();
        FunctionManager fm = currentProgram.getFunctionManager();

        println("[*] Importing Splatoon 1 symbols from " + file.getAbsolutePath() + "...");
        int count = 0;
        int lineNum = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = br.readLine()) != null) {
                lineNum++;
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")) continue;

                int spaceIdx = line.indexOf(' ');
                if (spaceIdx == -1) continue;

                String addrStr = line.substring(0, spaceIdx).trim();
                String name = line.substring(spaceIdx + 1).trim();

                try {
                    Address addr = toAddr(Long.parseLong(addrStr, 16));
                    if (addr != null) {
                        Function func = fm.getFunctionContaining(addr);
                        if (func != null && (func.getName().startsWith("FUN_") || func.getName().startsWith("fn_"))) {
                            func.setName(name, SourceType.USER_DEFINED);
                            count++;
                        } else {
                            st.createLabel(addr, name, SourceType.USER_DEFINED);
                            count++;
                        }
                    }
                } catch (Exception ex) {
                    // ignore invalid address conversions
                }

                if (count % 10000 == 0 && count > 0) {
                    println("  -> Processed " + count + " symbols so far...");
                }
            }
        }

        println("[+] Successfully imported " + count + " Splatoon 1 symbols into Ghidra!");
    }
}
