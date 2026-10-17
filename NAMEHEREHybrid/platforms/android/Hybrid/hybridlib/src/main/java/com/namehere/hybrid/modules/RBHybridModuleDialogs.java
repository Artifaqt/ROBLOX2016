package com.namehere.hybrid.modules;

import com.namehere.hybrid.RBHybridCommand;
import com.namehere.hybrid.RBHybridModule;

/**
 * Created by namehere on 3/6/15.
 */
public class RBHybridModuleDialogs extends RBHybridModule {
    private static final String MODULE_ID = "Dialogs";

    public RBHybridModuleDialogs() {
        super(MODULE_ID);
    }

    @Override
    public void execute(RBHybridCommand command) {
        command.executeCallback(false, null);
    }
}
