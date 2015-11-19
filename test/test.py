import CEmergent as ce

if __name__ == '__main__':
    cam = ce.HsCam()
    print('-'*60)
    detect = cam.detect_camera()
    #print("Return: ",detect)
    print('-'*60)
    getcam = cam.get_camera(0)
    #print("Return: ", getcam)
    print('-'*60)
    
    stringParam = []
    boolParam = []
    enumParam = []
    
    # ----- For uint32Param ----- #
    with open('./uint32Param.conf') as f:
        stringParam = f.read().splitlines()

    for param in stringParam:
        fr = cam.param_range(param)
        #print("Return: ", fr)
        print ''
        
    # ----- For boolParam ----- #
    print '-'*60
    with open('./boolParam.conf') as f:
        boolParam = f.read().splitlines()

    for param in boolParam:
        pr = cam.param_range(param)
        #print("Return: ", pr)
        print ''
    # ----- For EnumParam ----- #
    print '-'*60
    with open('./enumParam.conf') as f:
        enumParam = f.read().splitlines()

    for param in enumParam:
        er = cam.param_range(param)
        #print("Return: ", er)
        print ''
