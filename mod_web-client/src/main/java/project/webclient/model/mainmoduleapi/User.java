package project.webclient.model.mainmoduleapi;

import lombok.Data;

@Data
public class User {
    private Long id;
    private String firstName;
    private String lastName;
    private String middleName;
    private Boolean banned;
}