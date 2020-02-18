package threadsocket;

public final class TelnetCommand
{
    /*** The maximum value a command code can have.  This value is 255. ***/
    public static final int MAX_COMMAND_VALUE = 255;

    /*** Interpret As Command code.  Value is 255 according to RFC 854. ***/
    public static final int IAC = 255;

    /*** Don't use option code.  Value is 254 according to RFC 854. ***/
    public static final int DONT = 254;

    /*** Request to use option code.  Value is 253 according to RFC 854. ***/
    public static final int DO = 253;

    /*** Refuse to use option code.  Value is 252 according to RFC 854. ***/
    public static final int WONT = 252;

    /*** Agree to use option code.  Value is 251 according to RFC 854. ***/
    public static final int WILL = 251;

    /*** Start sub negotiation code.  Value is 250 according to RFC 854. ***/
    public static final int SB = 250;

    /*** Go Ahead code.  Value is 249 according to RFC 854. ***/
    public static final int GA = 249;

    /*** Erase Line code.  Value is 248 according to RFC 854. ***/
    public static final int EL = 248;

    /*** Erase Character code.  Value is 247 according to RFC 854. ***/
    public static final int EC = 247;

    /*** Are You There code.  Value is 246 according to RFC 854. ***/
    public static final int AYT = 246;

    /*** Abort Output code.  Value is 245 according to RFC 854. ***/
    public static final int AO = 245;

    /*** Interrupt Process code.  Value is 244 according to RFC 854. ***/
    public static final int IP = 244;

    /*** Break code.  Value is 243 according to RFC 854. ***/
    public static final int BREAK = 243;

    /*** Data mark code.  Value is 242 according to RFC 854. ***/
    public static final int DM = 242;

    /*** No Operation code.  Value is 241 according to RFC 854. ***/
    public static final int NOP = 241;

    /*** End sub negotiation code.  Value is 240 according to RFC 854. ***/
    public static final int SE = 240;

    /*** Synchronize code.  Value is 242. according to RFC 854. ***/
    public static final int SYNCH = 242;
    

    /*** End of file code.  Value is 236. ***/
    public static final int EOF = -1;


    private TelnetCommand() {}
}