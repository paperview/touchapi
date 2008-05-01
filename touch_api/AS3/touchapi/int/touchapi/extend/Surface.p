
program 
  statementlist 
    config namespace  
      identifier CONFIG 
    package 
      packagename 
        packageidentifiers 
          identifier touchapi
          identifier extend
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier display
          identifier Sprite
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier Event
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier MouseEvent
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier KeyboardEvent
    import  
      packagename 
        packageidentifiers 
          identifier touchapi
          identifier *
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier TouchEvent
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier geom
          identifier Point
    class 
      attributelist 
        list 
          member 
            get lexical 
              identifier public
      qualifiedidentifier Surface 
        qualifier 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
      member 
        get lexical 
          identifier Sprite
      statementlist 
        var 
          attributelist 
            list 
              member 
                get lexical 
                  identifier protected 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier blobs 
                  qualifier 
                    attributelist 
                      list 
                        member 
                          get lexical 
                            identifier protected
                typeexpr 
                  member 
                    get lexical 
                      identifier Array
              member 
                construct lexical 
                  identifier Array
        var 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier passive 
                  qualifier 
                    attributelist 
                      list 
                        member 
                          get lexical 
                            identifier public
                typeexpr 
                  member 
                    get lexical 
                      identifier Boolean
              literalboolean 0
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier Surface 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
            statementlist 
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier Event
                          get dot 
                            identifier ADDED_TO_STAGE
                        member 
                          this
                          get dot 
                            identifier mtAddedToStage
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier Event
                          get dot 
                            identifier REMOVED_FROM_STAGE
                        member 
                          this
                          get dot 
                            identifier mtRemovedFromStage
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier getBlobInfo 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
              typeexpr 
                member 
                  get lexical 
                    identifier Object
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier i
                      typeexpr 
                        member 
                          get lexical 
                            identifier int
                    literalnumber:0
              for  
                list 
                  binary lessthan 
                    member 
                      get lexical 
                        identifier i
                    member 
                      member 
                        get lexical 
                          identifier blobs
                      get dot 
                        identifier length 
                list 
                  member 
                    increment lexical postfix plusplus  
                      identifier i 
                statementlist 
                  if 
                    list 
                      binary equals 
                        member 
                          member 
                            member 
                              get lexical 
                                identifier blobs
                            get bracket 
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i
                          get dot 
                            identifier id
                        member 
                          get lexical 
                            identifier id 
                    statementlist 
                      return 
                        list 
                          member 
                            member 
                              get lexical 
                                identifier blobs
                            get bracket 
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i 
              return 
                list 
                  literalnull
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier getStateInfo 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              typeexpr 
                member 
                  get lexical 
                    identifier Boolean
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier mtAddedToStage 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier Event
            statementlist 
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_MOVE
                        member 
                          this
                          get dot 
                            identifier moveHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_DOWN
                        member 
                          this
                          get dot 
                            identifier downHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_UP
                        member 
                          this
                          get dot 
                            identifier upHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_OVER
                        member 
                          this
                          get dot 
                            identifier rollOverHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_OUT
                        member 
                          this
                          get dot 
                            identifier rollOutHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier MouseEvent
                          get dot 
                            identifier MOUSE_MOVE
                        member 
                          this
                          get dot 
                            identifier mouseMoveHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier MouseEvent
                          get dot 
                            identifier MOUSE_DOWN
                        member 
                          this
                          get dot 
                            identifier mouseDownHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier MouseEvent
                          get dot 
                            identifier MOUSE_UP
                        member 
                          this
                          get dot 
                            identifier mouseUpHandler
                        literalboolean 0
                        literalnumber:0
                        literalboolean 1
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier mtRemovedFromStage 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier Event
            statementlist 
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_MOVE
                        member 
                          this
                          get dot 
                            identifier moveHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_DOWN
                        member 
                          this
                          get dot 
                            identifier downHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_UP
                        member 
                          this
                          get dot 
                            identifier upHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_OVER
                        member 
                          this
                          get dot 
                            identifier rollOverHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier TouchEvent
                          get dot 
                            identifier MOUSE_OUT
                        member 
                          this
                          get dot 
                            identifier rollOutHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier MouseEvent
                          get dot 
                            identifier MOUSE_MOVE
                        member 
                          this
                          get dot 
                            identifier mouseMoveHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier MouseEvent
                          get dot 
                            identifier MOUSE_DOWN
                        member 
                          this
                          get dot 
                            identifier mouseDownHandler
              expression 
                list 
                  member 
                    this
                    call dot 
                      identifier removeEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier MouseEvent
                          get dot 
                            identifier MOUSE_UP
                        member 
                          this
                          get dot 
                            identifier mouseUpHandler
              expression 
                list 
                  member 
                    call lexical 
                      identifier trace
                      argumentlist 
                        literalstring:SURFACE DESTROYED -----------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier idExists 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
              typeexpr 
                member 
                  get lexical 
                    identifier Boolean
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier i
                      typeexpr 
                        member 
                          get lexical 
                            identifier int
                    literalnumber:0
              for  
                list 
                  binary lessthan 
                    member 
                      get lexical 
                        identifier i
                    member 
                      member 
                        get lexical 
                          identifier blobs
                      get dot 
                        identifier length 
                list 
                  member 
                    increment lexical postfix plusplus  
                      identifier i 
                statementlist 
                  if 
                    list 
                      binary equals 
                        member 
                          member 
                            member 
                              get lexical 
                                identifier blobs
                            get bracket 
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i
                          get dot 
                            identifier id
                        member 
                          get lexical 
                            identifier id 
                    statementlist 
                      return 
                        list 
                          literalboolean 1 
              return 
                list 
                  literalboolean 0
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier addBlob 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier origX
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier origY
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier c
                  typeexpr 
                    member 
                      get lexical 
                        identifier Boolean
            statementlist 
              if 
                list 
                  member 
                    call lexical 
                      identifier idExists
                      argumentlist 
                        member 
                          get lexical 
                            identifier id 
                statementlist 
                  return  
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier blobs
                    call dot 
                      identifier push
                      argumentlist 
                        literalobject 
                          argumentlist 
                            literalfield 
                              identifier id
                              member 
                                get lexical 
                                  identifier id
                            literalfield 
                              identifier clicked
                              member 
                                get lexical 
                                  identifier c
                            literalfield 
                              identifier origX
                              member 
                                get lexical 
                                  identifier origX
                            literalfield 
                              identifier origY
                              member 
                                get lexical 
                                  identifier origY
                            literalfield 
                              identifier clicked
                              member 
                                get lexical 
                                  identifier c
                            literalfield 
                              identifier history
                              member 
                                construct lexical 
                                  identifier Array
                                  argumentlist 
                                    member 
                                      construct lexical 
                                        identifier Point
                                        argumentlist 
                                          member 
                                            get lexical 
                                              identifier origX
                                          member 
                                            get lexical 
                                              identifier origY
                            literalfield 
                              identifier dX
                              literalnumber:0.0
                            literalfield 
                              identifier dY
                              literalnumber:0.0
                            literalfield 
                              identifier x
                              member 
                                get lexical 
                                  identifier origX
                            literalfield 
                              identifier y
                              member 
                                get lexical 
                                  identifier origY
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleBlobCreated
                      argumentlist 
                        member 
                          get lexical 
                            identifier id
                        member 
                          get lexical 
                            identifier origX
                        member 
                          get lexical 
                            identifier origY
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier removeBlob 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier i
                      typeexpr 
                        member 
                          get lexical 
                            identifier int
                    literalnumber:0
              for  
                list 
                  binary lessthan 
                    member 
                      get lexical 
                        identifier i
                    member 
                      member 
                        get lexical 
                          identifier blobs
                      get dot 
                        identifier length 
                list 
                  member 
                    increment lexical postfix plusplus  
                      identifier i 
                statementlist 
                  if 
                    list 
                      binary equals 
                        member 
                          member 
                            member 
                              get lexical 
                                identifier blobs
                            get bracket 
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i
                          get dot 
                            identifier id
                        member 
                          get lexical 
                            identifier id 
                    statementlist 
                      expression 
                        list 
                          member 
                            call lexical 
                              identifier handleBlobRemoved
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier id
                                literalnumber:0
                                literalnumber:0
                      expression 
                        list 
                          member 
                            member 
                              get lexical 
                                identifier blobs
                            call dot 
                              identifier splice
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i
                                literalnumber:1
                      return  
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier updateBlob 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier origX
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier origY
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier i
                      typeexpr 
                        member 
                          get lexical 
                            identifier int
                    literalnumber:0
              for 
                list 
                  member 
                    set lexical 
                      identifier i
                      argumentlist 
                        literalnumber:0 
                list 
                  binary lessthan 
                    member 
                      get lexical 
                        identifier i
                    member 
                      member 
                        get lexical 
                          identifier blobs
                      get dot 
                        identifier length 
                list 
                  member 
                    increment lexical postfix plusplus  
                      identifier i 
                statementlist 
                  if 
                    list 
                      binary equals 
                        member 
                          member 
                            member 
                              get lexical 
                                identifier blobs
                            get bracket 
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i
                          get dot 
                            identifier id
                        member 
                          get lexical 
                            identifier id 
                    statementlist 
                      expression 
                        list 
                          member 
                            member 
                              member 
                                member 
                                  get lexical 
                                    identifier blobs
                                get bracket 
                                  argumentlist 
                                    member 
                                      get lexical 
                                        identifier i
                              get dot 
                                identifier history
                            call dot 
                              identifier push
                              argumentlist 
                                member 
                                  construct lexical 
                                    identifier Point
                                    argumentlist 
                                      member 
                                        get lexical 
                                          identifier origX
                                      member 
                                        get lexical 
                                          identifier origY
                      expression 
                        list 
                          member 
                            member 
                              member 
                                get lexical 
                                  identifier blobs
                              get bracket 
                                argumentlist 
                                  member 
                                    get lexical 
                                      identifier i
                            set dot 
                              identifier x
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier origX
                      expression 
                        list 
                          member 
                            member 
                              member 
                                get lexical 
                                  identifier blobs
                              get bracket 
                                argumentlist 
                                  member 
                                    get lexical 
                                      identifier i
                            set dot 
                              identifier y
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier origY
                      if 
                        list 
                          binary greaterthanorequals 
                            member 
                              member 
                                member 
                                  member 
                                    get lexical 
                                      identifier blobs
                                  get bracket 
                                    argumentlist 
                                      member 
                                        get lexical 
                                          identifier i
                                get dot 
                                  identifier history
                              get dot 
                                identifier length
                            literalnumber:2 
                        statementlist 
                          var  
                            list 
                              variablebinding 
                                typedidentifier 
                                  qualifiedidentifier len
                                  typeexpr 
                                    member 
                                      get lexical 
                                        identifier int
                                member 
                                  member 
                                    member 
                                      member 
                                        get lexical 
                                          identifier blobs
                                      get bracket 
                                        argumentlist 
                                          member 
                                            get lexical 
                                              identifier i
                                    get dot 
                                      identifier history
                                  get dot 
                                    identifier length
                          expression 
                            list 
                              member 
                                member 
                                  member 
                                    get lexical 
                                      identifier blobs
                                  get bracket 
                                    argumentlist 
                                      member 
                                        get lexical 
                                          identifier i
                                set dot 
                                  identifier dX
                                  argumentlist 
                                    binary minus 
                                      member 
                                        member 
                                          member 
                                            member 
                                              member 
                                                get lexical 
                                                  identifier blobs
                                              get bracket 
                                                argumentlist 
                                                  member 
                                                    get lexical 
                                                      identifier i
                                            get dot 
                                              identifier history
                                          get bracket 
                                            argumentlist 
                                              binary minus 
                                                member 
                                                  get lexical 
                                                    identifier len
                                                literalnumber:1
                                        get dot 
                                          identifier x
                                      member 
                                        member 
                                          member 
                                            member 
                                              member 
                                                get lexical 
                                                  identifier blobs
                                              get bracket 
                                                argumentlist 
                                                  member 
                                                    get lexical 
                                                      identifier i
                                            get dot 
                                              identifier history
                                          get bracket 
                                            argumentlist 
                                              binary minus 
                                                member 
                                                  get lexical 
                                                    identifier len
                                                literalnumber:2
                                        get dot 
                                          identifier x
                          expression 
                            list 
                              member 
                                member 
                                  member 
                                    get lexical 
                                      identifier blobs
                                  get bracket 
                                    argumentlist 
                                      member 
                                        get lexical 
                                          identifier i
                                set dot 
                                  identifier dY
                                  argumentlist 
                                    binary minus 
                                      member 
                                        member 
                                          member 
                                            member 
                                              member 
                                                get lexical 
                                                  identifier blobs
                                              get bracket 
                                                argumentlist 
                                                  member 
                                                    get lexical 
                                                      identifier i
                                            get dot 
                                              identifier history
                                          get bracket 
                                            argumentlist 
                                              binary minus 
                                                member 
                                                  get lexical 
                                                    identifier len
                                                literalnumber:1
                                        get dot 
                                          identifier y
                                      member 
                                        member 
                                          member 
                                            member 
                                              member 
                                                get lexical 
                                                  identifier blobs
                                              get bracket 
                                                argumentlist 
                                                  member 
                                                    get lexical 
                                                      identifier i
                                            get dot 
                                              identifier history
                                          get bracket 
                                            argumentlist 
                                              binary minus 
                                                member 
                                                  get lexical 
                                                    identifier len
                                                literalnumber:2
                                        get dot 
                                          identifier y 
                      return  
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier downHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier TouchEvent
            statementlist 
              if 
                list 
                  binary logicaland 
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageX
                      literalnumber:0
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageY
                      literalnumber:0 
                statementlist 
                  return  
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier TouchAPI
                    call dot 
                      identifier addObjectListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        this
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              expression 
                list 
                  member 
                    call lexical 
                      identifier addBlob
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        literalboolean 1
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleDownEvent
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier target
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier rollOverHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier TouchEvent
            statementlist 
              if 
                list 
                  binary logicaland 
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageX
                      literalnumber:0
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageY
                      literalnumber:0 
                statementlist 
                  return  
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              if 
                list 
                  unary not 
                    member 
                      call lexical 
                        identifier idExists
                        argumentlist 
                          member 
                            member 
                              get lexical 
                                identifier e
                            get dot 
                              identifier ID 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier TouchAPI
                        call dot 
                          identifier addObjectListener
                          argumentlist 
                            member 
                              member 
                                get lexical 
                                  identifier e
                              get dot 
                                identifier ID
                            this
                  expression 
                    list 
                      member 
                        call lexical 
                          identifier addBlob
                          argumentlist 
                            member 
                              member 
                                get lexical 
                                  identifier e
                              get dot 
                                identifier ID
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier x
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier y
                            literalboolean 0 
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleRollOverEvent
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier upHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier TouchEvent
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleUpEvent
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
              expression 
                list 
                  member 
                    call lexical 
                      identifier removeBlob
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier moveHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier TouchEvent
            statementlist 
              if 
                list 
                  binary logicaland 
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageX
                      literalnumber:0
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageY
                      literalnumber:0 
                statementlist 
                  return  
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              if 
                list 
                  unary not 
                    member 
                      call lexical 
                        identifier idExists
                        argumentlist 
                          member 
                            member 
                              get lexical 
                                identifier e
                            get dot 
                              identifier ID 
                statementlist 
                  expression 
                    list 
                      member 
                        call lexical 
                          identifier addBlob
                          argumentlist 
                            member 
                              member 
                                get lexical 
                                  identifier e
                              get dot 
                                identifier ID
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier x
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier y
                            literalboolean 0 
                statementlist 
                  expression 
                    list 
                      member 
                        call lexical 
                          identifier updateBlob
                          argumentlist 
                            member 
                              member 
                                get lexical 
                                  identifier e
                              get dot 
                                identifier ID
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier x
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier y
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleMoveEvent
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier target
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier mouseDownHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier MouseEvent
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              expression 
                list 
                  member 
                    call lexical 
                      identifier addBlob
                      argumentlist 
                        literalnumber:0
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        literalboolean 1
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleDownEvent
                      argumentlist 
                        literalnumber:0
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier target
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier mouseMoveHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier MouseEvent
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              if 
                list 
                  unary not 
                    member 
                      call lexical 
                        identifier idExists
                        argumentlist 
                          literalnumber:0 
                statementlist 
                  if 
                    list 
                      unary not 
                        member 
                          get lexical 
                            identifier passive 
                    statementlist 
                      expression 
                        list 
                          member 
                            member 
                              get lexical 
                                identifier e
                            call dot 
                              identifier stopPropagation 
                  return  
                statementlist 
                  expression 
                    list 
                      member 
                        call lexical 
                          identifier updateBlob
                          argumentlist 
                            literalnumber:0
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier x
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier y
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleMoveEvent
                      argumentlist 
                        literalnumber:0
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier target
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier stageMouseMoveHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier MouseEvent
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              if 
                list 
                  unary not 
                    member 
                      call lexical 
                        identifier idExists
                        argumentlist 
                          literalnumber:0 
                statementlist 
                  if 
                    list 
                      unary not 
                        member 
                          get lexical 
                            identifier passive 
                    statementlist 
                      expression 
                        list 
                          member 
                            member 
                              get lexical 
                                identifier e
                            call dot 
                              identifier stopPropagation 
                  return  
                statementlist 
                  expression 
                    list 
                      member 
                        call lexical 
                          identifier updateBlob
                          argumentlist 
                            literalnumber:0
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier x
                            member 
                              member 
                                get lexical 
                                  identifier curPt
                              get dot 
                                identifier y
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleMoveEvent
                      argumentlist 
                        literalnumber:0
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier target
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier mouseUpHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier MouseEvent
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleUpEvent
                      argumentlist 
                        literalnumber:0
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
              expression 
                list 
                  member 
                    call lexical 
                      identifier removeBlob
                      argumentlist 
                        literalnumber:0
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier rollOutHandler 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier e
                  typeexpr 
                    member 
                      get lexical 
                        identifier TouchEvent
            statementlist 
              if 
                list 
                  binary logicaland 
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageX
                      literalnumber:0
                    binary equals 
                      member 
                        member 
                          get lexical 
                            identifier e
                        get dot 
                          identifier stageY
                      literalnumber:0 
                statementlist 
                  return  
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier curPt
                      typeexpr 
                        member 
                          get lexical 
                            identifier Point
                    member 
                      this
                      call dot 
                        identifier globalToLocal
                        argumentlist 
                          member 
                            construct lexical 
                              identifier Point
                              argumentlist 
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageX
                                member 
                                  member 
                                    get lexical 
                                      identifier e
                                  get dot 
                                    identifier stageY
              expression 
                list 
                  member 
                    call lexical 
                      identifier handleRollOutEvent
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier x
                        member 
                          member 
                            get lexical 
                              identifier curPt
                          get dot 
                            identifier y
              expression 
                list 
                  member 
                    call lexical 
                      identifier removeBlob
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier e
                          get dot 
                            identifier ID
              if 
                list 
                  unary not 
                    member 
                      get lexical 
                        identifier passive 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier e
                        call dot 
                          identifier stopPropagation 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleBlobCreated 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleBlobRemoved 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleDownEvent 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleUpEvent 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleRollOverEvent 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleRollOutEvent 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
        function 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier handleMoveEvent 
              qualifier 
                attributelist 
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier id
                  typeexpr 
                    member 
                      get lexical 
                        identifier int
                parameter 
                  identifier mx
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier my
                  literalnumber:0
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier targetObj
                  literalnull
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              return 
    package 
      packagename 
        packageidentifiers 
          identifier touchapi
          identifier extend
