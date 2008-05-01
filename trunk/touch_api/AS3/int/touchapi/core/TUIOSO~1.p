
program 
  statementlist 
    config namespace  
      identifier CONFIG 
    package 
      packagename 
        packageidentifiers 
          identifier touchapi
          identifier core
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier display
          identifier Stage
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier display
          identifier DisplayObject
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier display
          identifier DisplayObjectContainer
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier DataEvent
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
          identifier geom
          identifier Point
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier net
          identifier XMLSocket
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
          identifier events
          identifier ConsoleEvent
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier IOErrorEvent
    import  
      packagename 
        packageidentifiers 
          identifier flash
          identifier events
          identifier SecurityErrorEvent
    class 
      attributelist 
        list 
          member 
            get lexical 
              identifier public
      qualifiedidentifier TUIOSocket 
        qualifier 
          attributelist 
            list 
              member 
                get lexical 
                  identifier public
      statementlist 
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier internal 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier state 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier internal
                typeexpr 
                  member 
                    get lexical 
                      identifier Boolean
              literalboolean 0
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier internal 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier debug 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier internal
                typeexpr 
                  member 
                    get lexical 
                      identifier Boolean
              literalboolean 1
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier private 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier initialized 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier private
                typeexpr 
                  member 
                    get lexical 
                      identifier Boolean
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier private 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier n_stage 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier private
                typeexpr 
                  member 
                    get lexical 
                      identifier Stage
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier private 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier n_socket 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier private
                typeexpr 
                  member 
                    get lexical 
                      identifier XMLSocket
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier private 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier n_ids 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier private
                typeexpr 
                  member 
                    get lexical 
                      identifier Array
              member 
                construct lexical 
                  identifier Array
        var 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier private 
          list 
            variablebinding 
              typedidentifier 
                qualifiedidentifier n_objects 
                  qualifier 
                    attributelist 
                      member 
                        get lexical 
                          identifier static
                      list 
                        member 
                          get lexical 
                            identifier private
                typeexpr 
                  member 
                    get lexical 
                      identifier Array
              member 
                construct lexical 
                  identifier Array
        function 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier init 
              qualifier 
                attributelist 
                  member 
                    get lexical 
                      identifier static
                  list 
                    member 
                      get lexical 
                        identifier public
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier $stage
                  typeexpr 
                    member 
                      get lexical 
                        identifier DisplayObjectContainer
                parameter 
                  identifier $debug
                  literalboolean 1
                  typeexpr 
                    member 
                      get lexical 
                        identifier Boolean
                parameter 
                  identifier $host
                  literalstring:localhost
                  typeexpr 
                    member 
                      get lexical 
                        identifier String
                parameter 
                  identifier $port
                  literalnumber:3000
                  typeexpr 
                    member 
                      get lexical 
                        identifier Number
                parameter 
                  identifier $url
                  literalstring:
                  typeexpr 
                    member 
                      get lexical 
                        identifier String
            statementlist 
              if 
                list 
                  member 
                    get lexical 
                      identifier initialized 
                statementlist 
                  return  
              expression 
                list 
                  member 
                    set lexical 
                      identifier initialized
                      argumentlist 
                        literalboolean 1
              expression 
                list 
                  member 
                    set lexical 
                      identifier debug
                      argumentlist 
                        member 
                          get lexical 
                            identifier $debug
              expression 
                list 
                  member 
                    set lexical 
                      identifier n_stage
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier $stage
                          get dot 
                            identifier stage
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier n_stage
                    set dot 
                      identifier align
                      argumentlist 
                        literalstring:TL
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier n_stage
                    set dot 
                      identifier scaleMode
                      argumentlist 
                        literalstring:noScale
              expression 
                list 
                  member 
                    set lexical 
                      identifier n_socket
                      argumentlist 
                        member 
                          construct lexical 
                            identifier XMLSocket
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier n_socket
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier DataEvent
                          get dot 
                            identifier DATA
                        member 
                          get lexical 
                            identifier dataHandler
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier n_socket
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier IOErrorEvent
                          get dot 
                            identifier IO_ERROR
                        member 
                          get lexical 
                            identifier ioErrorHandler
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier n_socket
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier SecurityErrorEvent
                          get dot 
                            identifier SECURITY_ERROR
                        member 
                          get lexical 
                            identifier securityErrorHandler
              expression 
                list 
                  member 
                    member 
                      get lexical 
                        identifier n_socket
                    call dot 
                      identifier addEventListener
                      argumentlist 
                        member 
                          member 
                            get lexical 
                              identifier Event
                          get dot 
                            identifier CONNECT
                        member 
                          get lexical 
                            identifier connectHandler
              try 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier n_socket
                        call dot 
                          identifier connect
                          argumentlist 
                            member 
                              get lexical 
                                identifier $host
                            member 
                              get lexical 
                                identifier $port 
                statementlist 
                  catch 
                    parameter 
                      identifier e
                      typeexpr 
                        member 
                          get lexical 
                            identifier Error  
              return 
        function 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier clear 
              qualifier 
                attributelist 
                  member 
                    get lexical 
                      identifier static
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
                    set lexical 
                      identifier n_ids
                      argumentlist 
                        member 
                          construct lexical 
                            identifier Array
              expression 
                list 
                  member 
                    set lexical 
                      identifier n_objects
                      argumentlist 
                        member 
                          construct lexical 
                            identifier Array
              return 
        function 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier getObjectById 
              qualifier 
                attributelist 
                  member 
                    get lexical 
                      identifier static
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
                        identifier Number
              typeexpr 
                member 
                  get lexical 
                    identifier TUIOObject
            statementlist 
              if 
                list 
                  binary equals 
                    member 
                      get lexical 
                        identifier id
                    literalnumber:0 
                statementlist 
                  return 
                    list 
                      member 
                        construct lexical 
                          identifier TUIOObject
                          argumentlist 
                            literalstring:mouse
                            literalnumber:0
                            member 
                              member 
                                get lexical 
                                  identifier n_stage
                              get dot 
                                identifier mouseX
                            member 
                              member 
                                get lexical 
                                  identifier n_stage
                              get dot 
                                identifier mouseY
                            literalnumber:0
                            literalnumber:0
                            literalnumber:0
                            literalnumber:0
                            literalnumber:10
                            literalnumber:10
                            literalnull 
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
                          identifier n_objects
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
                                identifier n_objects
                            get bracket 
                              argumentlist 
                                member 
                                  get lexical 
                                    identifier i
                          get dot 
                            identifier ID
                        member 
                          get lexical 
                            identifier id 
                    statementlist 
                      return 
                        list 
                          member 
                            member 
                              get lexical 
                                identifier n_objects
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
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier addObjectListener 
              qualifier 
                attributelist 
                  member 
                    get lexical 
                      identifier static
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
                        identifier Number
                parameter 
                  identifier reciever
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier tmpObj
                      typeexpr 
                        member 
                          get lexical 
                            identifier TUIOObject
                    member 
                      call lexical 
                        identifier getObjectById
                        argumentlist 
                          member 
                            get lexical 
                              identifier id
              if 
                list 
                  member 
                    get lexical 
                      identifier tmpObj 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier tmpObj
                        call dot 
                          identifier addListener
                          argumentlist 
                            member 
                              get lexical 
                                identifier reciever 
              return 
        function 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier public
          functionname 
            qualifiedidentifier removeObjectListener 
              qualifier 
                attributelist 
                  member 
                    get lexical 
                      identifier static
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
                        identifier Number
                parameter 
                  identifier reciever
                  typeexpr 
                    member 
                      get lexical 
                        identifier Object
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier tmpObj
                      typeexpr 
                        member 
                          get lexical 
                            identifier TUIOObject
                    member 
                      call lexical 
                        identifier getObjectById
                        argumentlist 
                          member 
                            get lexical 
                              identifier id
              if 
                list 
                  member 
                    get lexical 
                      identifier tmpObj 
                statementlist 
                  expression 
                    list 
                      member 
                        member 
                          get lexical 
                            identifier tmpObj
                        call dot 
                          identifier removeListener
                          argumentlist 
                            member 
                              get lexical 
                                identifier reciever 
              return 
        function 
          attributelist 
            member 
              get lexical 
                identifier static
            list 
              member 
                get lexical 
                  identifier private
          functionname 
            qualifiedidentifier processMessage 
              qualifier 
                attributelist 
                  member 
                    get lexical 
                      identifier static
                  list 
                    member 
                      get lexical 
                        identifier private
          functioncommon 
            functionsignature 
              parameterlist 
                parameter 
                  identifier msg
                  typeexpr 
                    member 
                      get lexical 
                        identifier XML
            statementlist 
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier fseq
                      typeexpr 
                        member 
                          get lexical 
                            identifier String
              var  
                list 
                  variablebinding 
                    typedidentifier 
                      qualifiedidentifier node
                      typeexpr 
                        member 
                          get lexical 
                            identifier XML
              for 
                list 
                  storeregister 
                    register -1 
                      literalnumber:0

                    storeregister 
                      register -1 
                        coerce 
                          list 
                            member 
                              member 
                                get lexical 
                                  identifier msg
                              get dot 
                                identifier MESSAGE
 
                    hasNext 
                      register -1 
                        register -1
  
                        statementlist 
                          expression 
                            member 
                              set lexical 
                                identifier node
                                argumentlist 
                                  member 
                                    loadregister 
                                      register -1
                                      invoke dot [[NextValue]]
                                        argumentlist 
                                          loadregister 
                                            register -1
                              statementlist 
                                if 
                                  list 
                                    member 
                                      member 
                                        member 
                                          get lexical 
                                            identifier node
                                        get dot 
                                          identifier ARGUMENT
                                      get bracket 
                                        argumentlist 
                                          literalnumber:0 
                                  statementlist 
                                    if 
                                      list 
                                        binary equals 
                                          member 
                                            member 
                                              member 
                                                member 
                                                  get lexical 
                                                    identifier node
                                                get dot 
                                                  identifier ARGUMENT
                                              get bracket 
                                                argumentlist 
                                                  literalnumber:0
                                            get dot 
                                              attributeidentifier VALUE
                                          literalstring:fseq 
                                      statementlist 
                                        expression 
                                          list 
                                            member 
                                              set lexical 
                                                identifier fseq
                                                argumentlist 
                                                  member 
                                                    member 
                                                      member 
                                                        member 
                                                          get lexical 
                                                            identifier node
                                                        get dot 
                                                          identifier ARGUMENT
                                                      get bracket 
                                                        argumentlist 
                                                          literalnumber:1
                                                    get dot 
                                                      attributeidentifier VALUE 
                                    if 
                                      list 
                                        binary equals 
                                          member 
                                            member 
                                              member 
                                                member 
                                                  get lexical 
                                                    identifier node
                                                get dot 
                                                  identifier ARGUMENT
                                              get bracket 
                                                argumentlist 
                                                  literalnumber:0
                                            get dot 
                                              attributeidentifier VALUE
                                          literalstring:alive 
                                      statementlist 
                                        var  
                                          list 
                                            variablebinding 
                                              typedidentifier 
                                                qualifiedidentifier obj1
                                                typeexpr 
                                                  member 
                                                    get lexical 
                                                      identifier TUIOObject
                                        for 
                                          list 
                                            storeregister 
                                              register -1 
                                                literalnumber:0

                                              storeregister 
                                                register -1 
                                                  coerce 
                                                    list 
                                                      member 
                                                        get lexical 
                                                          identifier n_objects
 
                                              hasNext 
                                                register -1 
                                                  register -1
  
                                                  statementlist 
                                                    expression 
                                                      member 
                                                        set lexical 
                                                          qualifiedidentifier obj1
                                                          argumentlist 
                                                            member 
                                                              loadregister 
                                                                register -1
                                                                invoke dot [[NextValue]]
                                                                  argumentlist 
                                                                    loadregister 
                                                                      register -1
                                                        statementlist 
                                                          expression 
                                                            list 
                                                              member 
                                                                member 
                                                                  get lexical 
                                                                    identifier obj1
                                                                set dot 
                                                                  identifier alive
                                                                  argumentlist 
                                                                    literalboolean 0
                                                    var  
                                                      list 
                                                        variablebinding 
                                                          typedidentifier 
                                                            qualifiedidentifier newIdArray
                                                            typeexpr 
                                                              member 
                                                                get lexical 
                                                                  identifier Array
                                                          member 
                                                            construct lexical 
                                                              identifier Array
                                                    var  
                                                      list 
                                                        variablebinding 
                                                          typedidentifier 
                                                            qualifiedidentifier aliveItem
                                                            typeexpr 
                                                              member 
                                                                get lexical 
                                                                  identifier XML
                                                    for 
                                                      list 
                                                        storeregister 
                                                          register -1 
                                                            literalnumber:0

                                                          storeregister 
                                                            register -1 
                                                              coerce 
                                                                list 
                                                                  statementlist 
                                                                    register -1
                                                                      for 
                                                                        list 
                                                                          storeregister 
                                                                            register -1 
                                                                              literalnumber:0

                                                                            storeregister 
                                                                              register -1 
                                                                                coerce 
                                                                                  invoke dot [[CheckFilterOperand]]
                                                                                    argumentlist 
                                                                                      member 
                                                                                        member 
                                                                                          get lexical 
                                                                                            identifier node
                                                                                        get dot 
                                                                                          identifier ARGUMENT

                                                                              storeregister 
                                                                                register -1 
                                                                                  literalxml 
                                                                                    list 
                                                                                      literalstring:
 
                                                                              hasNext 
                                                                                register -1 
                                                                                  register -1
  
                                                                                  statementlist 
                                                                                    expression 
                                                                                      storeregister 
                                                                                        register -1 
                                                                                          member 
                                                                                            loadregister 
                                                                                              register -1
                                                                                              invoke dot [[NextValue]]
                                                                                                argumentlist 
                                                                                                  loadregister 
                                                                                                    register -1

                                                                                          with 
                                                                                            loadregister 
                                                                                              register -1 
                                                                                              statementlist 
                                                                                                if 
                                                                                                  list 
                                                                                                    list 
                                                                                                      binary notequals 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            attributeidentifier VALUE
                                                                                                        literalstring:alive 
                                                                                                  statementlist 
                                                                                                    expression 
                                                                                                      member 
                                                                                                        loadregister 
                                                                                                          register -1
                                                                                                          set lexical 
                                                                                                            loadregister 
                                                                                                              register -1
                                                                                                              argumentlist 
                                                                                                                loadregister 
                                                                                                                  register -1 
                                                                                              loadregister 
                                                                                                register -1
 
                                                                                      hasNext 
                                                                                        register -1 
                                                                                          register -1
  
                                                                                          statementlist 
                                                                                            expression 
                                                                                              member 
                                                                                                set lexical 
                                                                                                  qualifiedidentifier aliveItem
                                                                                                  argumentlist 
                                                                                                    member 
                                                                                                      loadregister 
                                                                                                        register -1
                                                                                                        invoke dot [[NextValue]]
                                                                                                          argumentlist 
                                                                                                            loadregister 
                                                                                                              register -1
                                                                                                statementlist 
                                                                                                  if 
                                                                                                    list 
                                                                                                      member 
                                                                                                        call lexical 
                                                                                                          identifier getObjectById
                                                                                                          argumentlist 
                                                                                                            member 
                                                                                                              member 
                                                                                                                get lexical 
                                                                                                                  identifier aliveItem
                                                                                                              get dot 
                                                                                                                attributeidentifier VALUE 
                                                                                                    statementlist 
                                                                                                      expression 
                                                                                                        list 
                                                                                                          member 
                                                                                                            member 
                                                                                                              call lexical 
                                                                                                                identifier getObjectById
                                                                                                                argumentlist 
                                                                                                                  member 
                                                                                                                    member 
                                                                                                                      get lexical 
                                                                                                                        identifier aliveItem
                                                                                                                    get dot 
                                                                                                                      attributeidentifier VALUE
                                                                                                            set dot 
                                                                                                              identifier alive
                                                                                                              argumentlist 
                                                                                                                literalboolean 1 
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier n_ids
                                                                                                    argumentlist 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier newIdArray 
                                                                                        var  
                                                                                          list 
                                                                                            variablebinding 
                                                                                              typedidentifier 
                                                                                                qualifiedidentifier type
                                                                                                typeexpr 
                                                                                                  member 
                                                                                                    get lexical 
                                                                                                      identifier String
                                                                                        expression 
                                                                                          list 
                                                                                            member 
                                                                                              set lexical 
                                                                                                identifier type
                                                                                                argumentlist 
                                                                                                  member 
                                                                                                    member 
                                                                                                      member 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier node
                                                                                                        get dot 
                                                                                                          identifier ARGUMENT
                                                                                                      get bracket 
                                                                                                        argumentlist 
                                                                                                          literalnumber:0
                                                                                                    get dot 
                                                                                                      attributeidentifier VALUE
                                                                                        if 
                                                                                          list 
                                                                                            binary equals 
                                                                                              member 
                                                                                                get lexical 
                                                                                                  identifier type
                                                                                              literalstring:set 
                                                                                          statementlist 
                                                                                            var  
                                                                                              list 
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier id
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier int
                                                                                            var  
                                                                                              list 
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier x
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier y
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier X
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier Y
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier m
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier wd
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                  literalnumber:0
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier ht
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Number
                                                                                                  literalnumber:0
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier id
                                                                                                    argumentlist 
                                                                                                      member 
                                                                                                        member 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier node
                                                                                                            get dot 
                                                                                                              identifier ARGUMENT
                                                                                                          get bracket 
                                                                                                            argumentlist 
                                                                                                              literalnumber:1
                                                                                                        get dot 
                                                                                                          attributeidentifier VALUE
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier x
                                                                                                    argumentlist 
                                                                                                      binary mult 
                                                                                                        member 
                                                                                                          call lexical 
                                                                                                            identifier Number
                                                                                                            argumentlist 
                                                                                                              member 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    member 
                                                                                                                      get lexical 
                                                                                                                        identifier node
                                                                                                                    get dot 
                                                                                                                      identifier ARGUMENT
                                                                                                                  get bracket 
                                                                                                                    argumentlist 
                                                                                                                      literalnumber:2
                                                                                                                get dot 
                                                                                                                  attributeidentifier VALUE
                                                                                                        member 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier n_stage
                                                                                                          get dot 
                                                                                                            identifier stageWidth
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier y
                                                                                                    argumentlist 
                                                                                                      binary mult 
                                                                                                        member 
                                                                                                          call lexical 
                                                                                                            identifier Number
                                                                                                            argumentlist 
                                                                                                              member 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    member 
                                                                                                                      get lexical 
                                                                                                                        identifier node
                                                                                                                    get dot 
                                                                                                                      identifier ARGUMENT
                                                                                                                  get bracket 
                                                                                                                    argumentlist 
                                                                                                                      literalnumber:3
                                                                                                                get dot 
                                                                                                                  attributeidentifier VALUE
                                                                                                        member 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier n_stage
                                                                                                          get dot 
                                                                                                            identifier stageHeight
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier X
                                                                                                    argumentlist 
                                                                                                      member 
                                                                                                        call lexical 
                                                                                                          identifier Number
                                                                                                          argumentlist 
                                                                                                            member 
                                                                                                              member 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier node
                                                                                                                  get dot 
                                                                                                                    identifier ARGUMENT
                                                                                                                get bracket 
                                                                                                                  argumentlist 
                                                                                                                    literalnumber:4
                                                                                                              get dot 
                                                                                                                attributeidentifier VALUE
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier Y
                                                                                                    argumentlist 
                                                                                                      member 
                                                                                                        call lexical 
                                                                                                          identifier Number
                                                                                                          argumentlist 
                                                                                                            member 
                                                                                                              member 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier node
                                                                                                                  get dot 
                                                                                                                    identifier ARGUMENT
                                                                                                                get bracket 
                                                                                                                  argumentlist 
                                                                                                                    literalnumber:5
                                                                                                              get dot 
                                                                                                                attributeidentifier VALUE
                                                                                            expression 
                                                                                              list 
                                                                                                member 
                                                                                                  set lexical 
                                                                                                    identifier m
                                                                                                    argumentlist 
                                                                                                      member 
                                                                                                        call lexical 
                                                                                                          identifier Number
                                                                                                          argumentlist 
                                                                                                            member 
                                                                                                              member 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier node
                                                                                                                  get dot 
                                                                                                                    identifier ARGUMENT
                                                                                                                get bracket 
                                                                                                                  argumentlist 
                                                                                                                    literalnumber:6
                                                                                                              get dot 
                                                                                                                attributeidentifier VALUE
                                                                                            if 
                                                                                              list 
                                                                                                member 
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier node
                                                                                                    get dot 
                                                                                                      identifier ARGUMENT
                                                                                                  get bracket 
                                                                                                    argumentlist 
                                                                                                      literalnumber:7 
                                                                                              statementlist 
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      set lexical 
                                                                                                        identifier wd
                                                                                                        argumentlist 
                                                                                                          binary mult 
                                                                                                            member 
                                                                                                              call lexical 
                                                                                                                identifier Number
                                                                                                                argumentlist 
                                                                                                                  member 
                                                                                                                    member 
                                                                                                                      member 
                                                                                                                        member 
                                                                                                                          get lexical 
                                                                                                                            identifier node
                                                                                                                        get dot 
                                                                                                                          identifier ARGUMENT
                                                                                                                      get bracket 
                                                                                                                        argumentlist 
                                                                                                                          literalnumber:7
                                                                                                                    get dot 
                                                                                                                      attributeidentifier VALUE
                                                                                                            member 
                                                                                                              member 
                                                                                                                get lexical 
                                                                                                                  identifier n_stage
                                                                                                              get dot 
                                                                                                                identifier stageWidth 
                                                                                            if 
                                                                                              list 
                                                                                                member 
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier node
                                                                                                    get dot 
                                                                                                      identifier ARGUMENT
                                                                                                  get bracket 
                                                                                                    argumentlist 
                                                                                                      literalnumber:8 
                                                                                              statementlist 
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      set lexical 
                                                                                                        identifier ht
                                                                                                        argumentlist 
                                                                                                          binary mult 
                                                                                                            member 
                                                                                                              call lexical 
                                                                                                                identifier Number
                                                                                                                argumentlist 
                                                                                                                  member 
                                                                                                                    member 
                                                                                                                      member 
                                                                                                                        member 
                                                                                                                          get lexical 
                                                                                                                            identifier node
                                                                                                                        get dot 
                                                                                                                          identifier ARGUMENT
                                                                                                                      get bracket 
                                                                                                                        argumentlist 
                                                                                                                          literalnumber:8
                                                                                                                    get dot 
                                                                                                                      attributeidentifier VALUE
                                                                                                            member 
                                                                                                              member 
                                                                                                                get lexical 
                                                                                                                  identifier n_stage
                                                                                                              get dot 
                                                                                                                identifier stageHeight 
                                                                                            var  
                                                                                              list 
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier stagePoint
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Point
                                                                                                  member 
                                                                                                    construct lexical 
                                                                                                      identifier Point
                                                                                                      argumentlist 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier x
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier y
                                                                                            var  
                                                                                              list 
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier displayObjArray
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier Array
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier n_stage
                                                                                                    call dot 
                                                                                                      identifier getObjectsUnderPoint
                                                                                                      argumentlist 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier stagePoint
                                                                                            var  
                                                                                              list 
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier dobj
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier DisplayObject
                                                                                                  literalnull
                                                                                            if 
                                                                                              list 
                                                                                                binary greaterthan 
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier displayObjArray
                                                                                                    get dot 
                                                                                                      identifier length
                                                                                                  literalnumber:0 
                                                                                              statementlist 
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      set lexical 
                                                                                                        identifier dobj
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier displayObjArray
                                                                                                            get bracket 
                                                                                                              argumentlist 
                                                                                                                binary minus 
                                                                                                                  member 
                                                                                                                    member 
                                                                                                                      get lexical 
                                                                                                                        identifier displayObjArray
                                                                                                                    get dot 
                                                                                                                      identifier length
                                                                                                                  literalnumber:1 
                                                                                            var  
                                                                                              list 
                                                                                                variablebinding 
                                                                                                  typedidentifier 
                                                                                                    qualifiedidentifier tuioobj
                                                                                                    typeexpr 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier TUIOObject
                                                                                                  member 
                                                                                                    call lexical 
                                                                                                      identifier getObjectById
                                                                                                      argumentlist 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier id
                                                                                            if 
                                                                                              list 
                                                                                                binary equals 
                                                                                                  member 
                                                                                                    get lexical 
                                                                                                      identifier tuioobj
                                                                                                  literalnull 
                                                                                              statementlist 
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier n_stage
                                                                                                      call dot 
                                                                                                        identifier dispatchEvent
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            construct lexical 
                                                                                                              identifier ConsoleEvent
                                                                                                              argumentlist 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier ConsoleEvent
                                                                                                                  get dot 
                                                                                                                    identifier WRITE
                                                                                                                literalboolean 1
                                                                                                                literalboolean 0
                                                                                                                literalstring:object 
                                                                                                                literalstring:-c touchapi.core.TUIOObject
                                                                                                                binary plus 
                                                                                                                  binary plus 
                                                                                                                    binary plus 
                                                                                                                      binary plus 
                                                                                                                        literalstring: (x:
                                                                                                                        member 
                                                                                                                          call lexical 
                                                                                                                            identifier int
                                                                                                                            argumentlist 
                                                                                                                              member 
                                                                                                                                get lexical 
                                                                                                                                  identifier x
                                                                                                                      literalstring:, y:
                                                                                                                    member 
                                                                                                                      call lexical 
                                                                                                                        identifier int
                                                                                                                        argumentlist 
                                                                                                                          member 
                                                                                                                            get lexical 
                                                                                                                              identifier y
                                                                                                                  literalstring:)
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      set lexical 
                                                                                                        identifier tuioobj
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            construct lexical 
                                                                                                              identifier TUIOObject
                                                                                                              argumentlist 
                                                                                                                literalstring:2Dcur
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier id
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier x
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier y
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier X
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier Y
                                                                                                                literalnumber:-1
                                                                                                                literalnumber:0
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier wd
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier ht
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier dobj
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier n_stage
                                                                                                      call dot 
                                                                                                        identifier addChild
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier tuioobj
                                                                                                            get dot 
                                                                                                              identifier cursor
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier n_objects
                                                                                                      call dot 
                                                                                                        identifier push
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier tuioobj
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      call dot 
                                                                                                        identifier notifyCreated 
                                                                                              statementlist 
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier tuioobj
                                                                                                        get dot 
                                                                                                          identifier cursor
                                                                                                      set dot 
                                                                                                        identifier x
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier x
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier tuioobj
                                                                                                        get dot 
                                                                                                          identifier cursor
                                                                                                      set dot 
                                                                                                        identifier y
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier y
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier oldX
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier tuioobj
                                                                                                            get dot 
                                                                                                              identifier x
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier oldY
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier tuioobj
                                                                                                            get dot 
                                                                                                              identifier y
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier x
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier x
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier y
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier y
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier n_stage
                                                                                                      call dot 
                                                                                                        identifier setChildIndex
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier tuioobj
                                                                                                            get dot 
                                                                                                              identifier cursor
                                                                                                          binary minus 
                                                                                                            member 
                                                                                                              member 
                                                                                                                get lexical 
                                                                                                                  identifier n_stage
                                                                                                              get dot 
                                                                                                                identifier numChildren
                                                                                                            literalnumber:1
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier width
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier wd
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier height
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier ht
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier area
                                                                                                        argumentlist 
                                                                                                          binary mult 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier wd
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier ht
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier dX
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier X
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      set dot 
                                                                                                        identifier dY
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier Y
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      call dot 
                                                                                                        identifier setObjOver
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier dobj
                                                                                                if 
                                                                                                  list 
                                                                                                    unary not 
                                                                                                      list 
                                                                                                        binary logicaland 
                                                                                                          binary equals 
                                                                                                            member 
                                                                                                              call lexical 
                                                                                                                identifier int
                                                                                                                argumentlist 
                                                                                                                  binary mult 
                                                                                                                    member 
                                                                                                                      get lexical 
                                                                                                                        identifier Y
                                                                                                                    literalnumber:1000
                                                                                                            literalnumber:0
                                                                                                          binary equals 
                                                                                                            member 
                                                                                                              call lexical 
                                                                                                                identifier int
                                                                                                                argumentlist 
                                                                                                                  binary mult 
                                                                                                                    member 
                                                                                                                      get lexical 
                                                                                                                        identifier Y
                                                                                                                    literalnumber:1000
                                                                                                            literalnumber:0 
                                                                                                  statementlist 
                                                                                                    expression 
                                                                                                      list 
                                                                                                        member 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier tuioobj
                                                                                                          call dot 
                                                                                                            identifier notifyMoved 
                                                                                            if 
                                                                                              list 
                                                                                                binary logicaland 
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier tuioobj
                                                                                                    get dot 
                                                                                                      identifier target
                                                                                                  member 
                                                                                                    member 
                                                                                                      member 
                                                                                                        get lexical 
                                                                                                          identifier tuioobj
                                                                                                      get dot 
                                                                                                        identifier target
                                                                                                    get dot 
                                                                                                      identifier parent 
                                                                                              statementlist 
                                                                                                var  
                                                                                                  list 
                                                                                                    variablebinding 
                                                                                                      typedidentifier 
                                                                                                        qualifiedidentifier localPoint
                                                                                                        typeexpr 
                                                                                                          member 
                                                                                                            get lexical 
                                                                                                              identifier Point
                                                                                                      member 
                                                                                                        member 
                                                                                                          member 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier tuioobj
                                                                                                            get dot 
                                                                                                              identifier target
                                                                                                          get dot 
                                                                                                            identifier parent
                                                                                                        call dot 
                                                                                                          identifier globalToLocal
                                                                                                          argumentlist 
                                                                                                            member 
                                                                                                              get lexical 
                                                                                                                identifier stagePoint
                                                                                                expression 
                                                                                                  list 
                                                                                                    member 
                                                                                                      member 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier tuioobj
                                                                                                        get dot 
                                                                                                          identifier target
                                                                                                      call dot 
                                                                                                        identifier dispatchEvent
                                                                                                        argumentlist 
                                                                                                          member 
                                                                                                            construct lexical 
                                                                                                              identifier TouchEvent
                                                                                                              argumentlist 
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier TouchEvent
                                                                                                                  get dot 
                                                                                                                    identifier MOUSE_MOVE
                                                                                                                literalboolean 1
                                                                                                                literalboolean 0
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier x
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier y
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier localPoint
                                                                                                                  get dot 
                                                                                                                    identifier x
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier localPoint
                                                                                                                  get dot 
                                                                                                                    identifier y
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier tuioobj
                                                                                                                  get dot 
                                                                                                                    identifier oldX
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier tuioobj
                                                                                                                  get dot 
                                                                                                                    identifier oldY
                                                                                                                member 
                                                                                                                  member 
                                                                                                                    get lexical 
                                                                                                                      identifier tuioobj
                                                                                                                  get dot 
                                                                                                                    identifier target
                                                                                                                literalboolean 0
                                                                                                                literalboolean 0
                                                                                                                literalboolean 0
                                                                                                                literalboolean 1
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier m
                                                                                                                literalstring:2Dcur
                                                                                                                member 
                                                                                                                  get lexical 
                                                                                                                    identifier id
                                                                                                                literalnumber:0
                                                                                                                literalnumber:0   
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
                                                                                          identifier n_objects
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
                                                                                                identifier n_objects
                                                                                            get bracket 
                                                                                              argumentlist 
                                                                                                member 
                                                                                                  get lexical 
                                                                                                    identifier i
                                                                                          get dot 
                                                                                            identifier alive
                                                                                        literalboolean 0 
                                                                                    statementlist 
                                                                                      expression 
                                                                                        list 
                                                                                          member 
                                                                                            member 
                                                                                              member 
                                                                                                get lexical 
                                                                                                  identifier n_objects
                                                                                              get bracket 
                                                                                                argumentlist 
                                                                                                  member 
                                                                                                    get lexical 
                                                                                                      identifier i
                                                                                            call dot 
                                                                                              identifier notifyRemoved
                                                                                      expression 
                                                                                        list 
                                                                                          member 
                                                                                            member 
                                                                                              get lexical 
                                                                                                identifier n_stage
                                                                                            call dot 
                                                                                              identifier removeChild
                                                                                              argumentlist 
                                                                                                member 
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier n_objects
                                                                                                    get bracket 
                                                                                                      argumentlist 
                                                                                                        member 
                                                                                                          get lexical 
                                                                                                            identifier i
                                                                                                  get dot 
                                                                                                    identifier cursor
                                                                                      expression 
                                                                                        list 
                                                                                          member 
                                                                                            member 
                                                                                              get lexical 
                                                                                                identifier n_objects
                                                                                            call dot 
                                                                                              identifier splice
                                                                                              argumentlist 
                                                                                                member 
                                                                                                  get lexical 
                                                                                                    identifier i
                                                                                                literalnumber:1
                                                                                      expression 
                                                                                        list 
                                                                                          member 
                                                                                            increment lexical postfix minusminus  
                                                                                              identifier i 
                                                                                    statementlist 
                                                                                      if 
                                                                                        list 
                                                                                          member 
                                                                                            get lexical 
                                                                                              identifier debug  
                                                                              return 
                                                                        function 
                                                                          attributelist 
                                                                            member 
                                                                              get lexical 
                                                                                identifier static
                                                                            list 
                                                                              member 
                                                                                get lexical 
                                                                                  identifier private
                                                                          functionname 
                                                                            qualifiedidentifier dataHandler 
                                                                              qualifier 
                                                                                attributelist 
                                                                                  member 
                                                                                    get lexical 
                                                                                      identifier static
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
                                                                                        identifier DataEvent
                                                                            statementlist 
                                                                              try 
                                                                                statementlist 
                                                                                  expression 
                                                                                    list 
                                                                                      member 
                                                                                        call lexical 
                                                                                          identifier processMessage
                                                                                          argumentlist 
                                                                                            member 
                                                                                              call lexical 
                                                                                                identifier XML
                                                                                                argumentlist 
                                                                                                  member 
                                                                                                    member 
                                                                                                      get lexical 
                                                                                                        identifier e
                                                                                                    get dot 
                                                                                                      identifier data 
                                                                                statementlist 
                                                                                  catch 
                                                                                    parameter 
                                                                                      identifier e
                                                                                      typeexpr 
                                                                                        member 
                                                                                          get lexical 
                                                                                            identifier Error  
                                                                              return 
                                                                        function 
                                                                          attributelist 
                                                                            member 
                                                                              get lexical 
                                                                                identifier static
                                                                            list 
                                                                              member 
                                                                                get lexical 
                                                                                  identifier private
                                                                          functionname 
                                                                            qualifiedidentifier ioErrorHandler 
                                                                              qualifier 
                                                                                attributelist 
                                                                                  member 
                                                                                    get lexical 
                                                                                      identifier static
                                                                                  list 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier private
                                                                          functioncommon 
                                                                            functionsignature 
                                                                              parameterlist 
                                                                                parameter 
                                                                                  identifier event
                                                                                  typeexpr 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier IOErrorEvent
                                                                            statementlist 
                                                                              return 
                                                                        function 
                                                                          attributelist 
                                                                            member 
                                                                              get lexical 
                                                                                identifier static
                                                                            list 
                                                                              member 
                                                                                get lexical 
                                                                                  identifier private
                                                                          functionname 
                                                                            qualifiedidentifier securityErrorHandler 
                                                                              qualifier 
                                                                                attributelist 
                                                                                  member 
                                                                                    get lexical 
                                                                                      identifier static
                                                                                  list 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier private
                                                                          functioncommon 
                                                                            functionsignature 
                                                                              parameterlist 
                                                                                parameter 
                                                                                  identifier event
                                                                                  typeexpr 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier SecurityErrorEvent
                                                                            statementlist 
                                                                              return 
                                                                        function 
                                                                          attributelist 
                                                                            member 
                                                                              get lexical 
                                                                                identifier static
                                                                            list 
                                                                              member 
                                                                                get lexical 
                                                                                  identifier private
                                                                          functionname 
                                                                            qualifiedidentifier connectHandler 
                                                                              qualifier 
                                                                                attributelist 
                                                                                  member 
                                                                                    get lexical 
                                                                                      identifier static
                                                                                  list 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier private
                                                                          functioncommon 
                                                                            functionsignature 
                                                                              parameterlist 
                                                                                parameter 
                                                                                  identifier event
                                                                                  typeexpr 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier Event
                                                                            statementlist 
                                                                              expression 
                                                                                list 
                                                                                  member 
                                                                                    member 
                                                                                      get lexical 
                                                                                        identifier n_stage
                                                                                    call dot 
                                                                                      identifier dispatchEvent
                                                                                      argumentlist 
                                                                                        member 
                                                                                          construct lexical 
                                                                                            identifier ConsoleEvent
                                                                                            argumentlist 
                                                                                              member 
                                                                                                member 
                                                                                                  get lexical 
                                                                                                    identifier ConsoleEvent
                                                                                                get dot 
                                                                                                  identifier WRITE
                                                                                              literalboolean 1
                                                                                              literalboolean 0
                                                                                              literalstring:TUIO Socket Connected...
                                                                              return 
                                                                    package 
                                                                      packagename 
                                                                        packageidentifiers 
                                                                          identifier touchapi
                                                                          identifier core
