class EX3Main(Actor): #Actor class inheritance
    scalefactor = 1.0
    def __init__(self):
        #Initialization
        self.Shoulder = Container(0) # Get Object information
        self.Arm = Container(1)
        self.Hand = Container(2)
        return
    def OnCreate(self, uid):
        #Constructor
        self.TransformGroup1 = self.Shoulder.FindComponentByType("TransformGroup") # Get Transform information
        self.TransformGroup2 = self.Arm.FindComponentByType("TransformGroup")
        self.TransformGroup3 = self.Hand.FindComponentByType("TransformGroup")
        return 0
        
    def OnDestroy(self):
        return 0
    def OnEnable(self):
        return 0
    def OnDisable(self):
        return 0
    def Update(self):
        #Updating frame
        return
    
    def OnMessage(self, msg, number, Vector4_lparm, Vector4_wparam):
        print(msg)

        if (msg == "KeyDown"):
            if( number == 0x51): #"Q"
                pos = Math3d.Vector3(0,3.5,0)
                fbxPos = self.TransformGroup1.GetPosition()
                fbxPos2 = self.TransformGroup2.GetPosition()
                fbxPos3 = self.TransformGroup3.GetPosition()   
                self.TransformGroup1.Rotate(0,-0.4,pos)
                self.TransformGroup2.Rotate(0,-0.3,fbxPos-Math3d.Vector3(0,0.25,0.5))#0,0.25,0.5
                self.TransformGroup3.Rotate(0,-0.2,fbxPos2-Math3d.Vector3(0,0.25,0.5))
                
            if( number == 0x57): #"W"
                pos = Math3d.Vector3(0,3.5,0)
                fbxPos = self.TransformGroup1.GetPosition()
                fbxPos2 = self.TransformGroup2.GetPosition()
                fbxPos3 = self.TransformGroup3.GetPosition()   
                self.TransformGroup1.Rotate(0,0.4,pos)
                self.TransformGroup2.Rotate(0,0.3,fbxPos-Math3d.Vector3(0,0.25,0.5))
                self.TransformGroup3.Rotate(0,0.2,fbxPos2-Math3d.Vector3(0,0.25,0.5))

            if( number == 0x45): #"E"
                pos = Math3d.Vector3(0,0,3)
                fbxPos = self.TransformGroup1.GetPosition()
                fbxPos2 = self.TransformGroup2.GetPosition()
                fbxPos3 = self.TransformGroup3.GetPosition()   
                self.TransformGroup1.Rotate(0,0,pos)
                self.TransformGroup2.Rotate(0,-0.3,fbxPos-Math3d.Vector3(0.25,0.5,0))
                self.TransformGroup3.Rotate(2,0,fbxPos2-Math3d.Vector3(0.25,0.25,0))
        return;