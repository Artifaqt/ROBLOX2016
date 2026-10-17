#import "NAMEHEREAlert.h"
#import <UIKit/UIKit.h>

@implementation NAMEHEREAlert
+(void) NAMEHEREAlertWithMessage:(NSString*) message
{
    dispatch_async(dispatch_get_main_queue(),^{
        /* open an alert with an OK button */
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"NAMEHEREWord", @"")
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"OkWord", nil)
                                              otherButtonTitles: nil];
        [alert show];
    });
}
+(void) NAMEHEREAlertWithMessageAndDelegate:(NSString*) message Delegate:(id) delegate
{
     dispatch_async(dispatch_get_main_queue(),^{
        /* open an alert with OK and Cancel buttons */
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"NAMEHEREWord", @"")
                                                        message:message
                                                       delegate:delegate
                                              cancelButtonTitle:NSLocalizedString(@"CancelWord", nil)
                                              otherButtonTitles:NSLocalizedString(@"OkWord", nil), nil];
        [alert show];
     });
}
+(void) NAMEHEREOKAlertWithMessageAndDelegate:(NSString*) message Delegate:(id) delegate
{
    dispatch_async(dispatch_get_main_queue(),^{
        /* open an alert with an OK button */
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"NAMEHEREWord", @"")
                                                        message:message
                                                       delegate:delegate
                                              cancelButtonTitle:NSLocalizedString(@"OkWord", nil)
                                              otherButtonTitles: nil];
        [alert show];
    });
}
@end
